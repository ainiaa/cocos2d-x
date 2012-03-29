#ifdef XBRICK
#include "helper.h"
#endif

#include "xbreader.h"

const CCPoint& VisibleRect::center()
{
	static CCPoint s_ptCenter;
	if (0 == s_ptCenter.x)
	{
		CCRect rc = rect();
		s_ptCenter.x = rc.origin.x + rc.size.width / 2;
		s_ptCenter.y = rc.origin.y + rc.size.height / 2;
	}
	return s_ptCenter;
}

const CCPoint& VisibleRect::top()
{
	static CCPoint s_ptTop;
	if (0 == s_ptTop.x)
	{
		CCRect rc = rect();
		s_ptTop.x = rc.origin.x + rc.size.width / 2;
		s_ptTop.y = rc.origin.y + rc.size.height;
	}
	return s_ptTop;
}

const CCPoint& VisibleRect::topRight()
{
	static CCPoint s_ptTopRight;
	if (0 == s_ptTopRight.x)
	{
		CCRect rc = rect();
		s_ptTopRight.x = rc.origin.x + rc.size.width;
		s_ptTopRight.y = rc.origin.y + rc.size.height;
	}
	return s_ptTopRight;
}

const CCPoint& VisibleRect::right()
{
	static CCPoint s_ptRight;
	if (0 == s_ptRight.x)
	{
		CCRect rc = rect();
		s_ptRight.x = rc.origin.x + rc.size.width;
		s_ptRight.y = rc.origin.y + rc.size.height / 2;
	}
	return s_ptRight;
}

const CCPoint& VisibleRect::bottomRight()
{
	static CCPoint s_ptBottomRight;
	if (0 == s_ptBottomRight.x)
	{
		CCRect rc = rect();
		s_ptBottomRight.x = rc.origin.x + rc.size.width;
		s_ptBottomRight.y = rc.origin.y;
	}
	return s_ptBottomRight;
}

const CCPoint& VisibleRect::bottom()
{
	static CCPoint s_ptBottom;
	if (0 == s_ptBottom.x)
	{
		CCRect rc = rect();
		s_ptBottom.x = rc.origin.x + rc.size.width / 2;
		s_ptBottom.y = rc.origin.y;
	}
	return s_ptBottom;
}

const CCPoint& VisibleRect::bottomLeft()
{
	return rect().origin;
}

const CCPoint& VisibleRect::left()
{
	static CCPoint s_ptLeft;
	if (0 == s_ptLeft.x)
	{
		CCRect rc = rect();
		s_ptLeft.x = rc.origin.x;
		s_ptLeft.y = rc.origin.y + rc.size.height / 2;
	}
	return s_ptLeft;
}

const CCPoint& VisibleRect::topLeft()
{
	static CCPoint s_ptTopLeft;
	if (0 == s_ptTopLeft.x)
	{
		CCRect rc = rect();
		s_ptTopLeft.x = rc.origin.x;
		s_ptTopLeft.y = rc.origin.y + rc.size.height;
	}
	return s_ptTopLeft;
}

static XBReader s_sharedReader;

XBReader* XBReader::sharedReader()
{
	return &s_sharedReader;
}

XBReader::XBReader() :
m_visibleRect(NULL)
{
}

CCPoint XBReader::getViewPointPosition(const char* viewPoint)
{
	CCAssert(m_visibleRect, "m_visibleRect must not be null");

	if (strcmp(viewPoint, "TopLeft") == 0)
	{
		return m_visibleRect->topLeft();
	}
	else if (strcmp(viewPoint, "Top") == 0)
	{
		return m_visibleRect->top();
	}
	else if (strcmp(viewPoint, "TopRight") == 0)
	{
		return m_visibleRect->topRight();
	}
	else if (strcmp(viewPoint, "Left") == 0)
	{
		return m_visibleRect->left();
	}
	else if (strcmp(viewPoint, "Center") == 0)
	{
		return m_visibleRect->center();
	}
	else if (strcmp(viewPoint, "Right") == 0)
	{
		return m_visibleRect->right();
	}
	else if (strcmp(viewPoint, "Bottom") == 0)
	{
		return m_visibleRect->bottom();
	}
	else if (strcmp(viewPoint, "BottomRight") == 0)
	{
		return m_visibleRect->bottomRight();
	}
	else
	{
		// the default case is BottomLeft
		return m_visibleRect->bottomLeft();
	}
}

CCNode* XBReader::nodeFromFile(const char *pFile)
{
	m_viewPoints.clear();
	m_viewPointOffset.clear();

	const char* szFullPath = CCFileUtils::fullPathFromRelativePath(pFile);
	CCDictionary<std::string, CCObject*> *dict = CCFileUtils::dictionaryWithContentsOfFileThreadSafe(szFullPath);

	CCAssert(dict != NULL, "XBReader file not found");

	XBDictionary *graph = (XBDictionary*)dict->objectForKey("graph");

	CCAssert(graph != NULL, "graph element is empty");
	CCNode* node = nodeFromDictionary(graph);
	dict->release();

	return node;
}

CCNode* XBReader::nodeFromDictionary(XBDictionary *dictionary)
{
	const char* className = valueForKey("class", dictionary);
	XBArray *children = (XBArray*)dictionary->objectForKey("children");
	XBDictionary *props = (XBDictionary*)dictionary->objectForKey("properties");

	CCNode* node = NULL;

	if (strcmp(className, "CCNode") == 0)
	{
		node = new CCNode;
		setPropForCCNode(node, props);
	} else if (strcmp(className, "CCSprite") == 0)
	{
		node = new CCSprite;
		CCSprite* sprite = dynamic_cast<CCSprite*>(node);

		setPropForSprite(sprite, props);
		setPropForCCNode(sprite, props);
	} else if (strcmp(className, "CCLayer") == 0)
	{
		node = new CCLayer;
		setPropForCCNode(node, props);
		setPropForCCLayer(dynamic_cast<CCLayer*>(node), props);
	} else if (strcmp(className, "CCLayerColor") == 0)
	{
		node = new CCLayerColor;

		CCLayerColor* layerColor = dynamic_cast<CCLayerColor*>(node);
		setPropForCCNode(layerColor, props);
		setPropForCCLayer(layerColor, props);
		setPropForCCLayerColor(layerColor, props);
	} else if (strcmp(className, "CCLayerGradient") == 0)
	{
		node = new CCLayerGradient;

		CCLayerGradient* layerGradient = dynamic_cast<CCLayerGradient*>(node);
		setPropForCCNode(layerGradient, props);
		setPropForCCLayer(layerGradient, props);
		setPropForCCLayerColor(layerGradient, props);
		setPropForCCLayerGradient(layerGradient, props);
	} else if (strcmp(className, "CCParticleSystem") == 0)
	{
		const char* plistFile = valueForKey("plistFile", props);

		node = new CCParticleSystemQuad;
		CCParticleSystem* particleSystem = dynamic_cast<CCParticleSystem*>(node);

		if (strcmp(plistFile, "") == 0)
		{
			// particleSystem only be edited by the other third party tool NOW
			return NULL;
			setPropForCCNode(node, props);
			setPropForCCParticleSystem(particleSystem, props);
		} else
		{
			// maybe the particlesystem is designed in the other third party tool
			// it will generate a .plist file
			setPropForCCNode(node, props);

			particleSystem->initWithFile(plistFile);

#ifdef XBRICK
			Helper::getSingleton().setParticleSystemFileName(particleSystem, plistFile);
#endif
		}
	} else if (strcmp(className, "CCMenu") == 0)
	{
		node = new CCMenu;
		CCMenu* menu = dynamic_cast<CCMenu*>(node);
		setPropForMenu(menu, props);
		setPropForCCNode(menu, props);
	} else if (strcmp(className, "CCMenuItemImage") == 0)
	{
		node = new CCMenuItemImage;

		CCMenuItemImage* itemImage = dynamic_cast<CCMenuItemImage*>(node);
		setPropForItemImage(itemImage, props);
		setPropForMenuItem(itemImage, props);
	} else if (strcmp(className, "CCLabelBMFont") == 0)
	{
		node = new CCLabelBMFont;
		CCLabelBMFont* bmFont = dynamic_cast<CCLabelBMFont*>(node);
		setPropForBMFont(bmFont, props);
		setPropForCCNode(node, props);
	} else
	{
		// unknown class
		CCLog("%s is not a know type to cocos2d-x.");
	}

	if (! node)
		return NULL;

	node->autorelease();

#ifdef XBRICK
	const char* nodeName = valueForKey("name", dictionary);
	Helper::getSingleton().setNodeName(node, nodeName);
#endif

	// Add children
	for (unsigned int i = 0; i < children->count(); i++)
	{
		XBDictionary* childDict = (XBDictionary*)children->getObjectAtIndex(i);
		CCNode* childNode = nodeFromDictionary(childDict);

		if (! childNode)
			continue;

		node->addChild(childNode, atoi(valueForKey("zOrder", childDict)));
	}

	return node;
}

void XBReader::relayout()
{
	std::map<CCNode*, std::string>::iterator iter;
	for (iter = m_viewPoints.begin(); iter != m_viewPoints.end(); iter++)
	{
		CCNode* node = iter->first;
		std::string viewPoint = iter->second;
		CCPoint offset = m_viewPointOffset[node];

		CCPoint viewPointPos = getViewPointPosition(viewPoint.c_str());
		CCPoint position = ccp(viewPointPos.x + offset.x, viewPointPos.y + offset.y);

		CCNode* parent = node->getParent();

		position = parent->convertToNodeSpace(position);
		node->setPosition(position);
	}
}

void XBReader::setPropForCCNode(CCNode* node, XBDictionary* props)
{
	std::string viewPoint = std::string(valueForKey("viewPoint", props));
	if (viewPoint.compare("") == 0)
	{
		viewPoint = "None";
		node->setPosition(pointFromDict("position", props));
	}
	else
	{
		// have set viewPoint
		m_viewPoints[node] = viewPoint;

		CCPoint offset = pointFromDict("offset", props);
		m_viewPointOffset[node] = offset;
	}

#ifdef XBRICK
	Helper::getSingleton().setNodeViewPoint(node, viewPoint.c_str());
#endif

	if (! dynamic_cast<CCSprite*>(node) &&
		! dynamic_cast<CCMenuItemImage*>(node) &&
		! dynamic_cast<CCLabelBMFont*>(node) &&
		! dynamic_cast<CCParticleSystem*>(node))
		node->setContentSize(sizeFromDict("contentSize", props));

	node->setScaleX((float)atof(valueForKey("scaleX", props)));
	node->setScaleY((float)atof(valueForKey("scaleY", props)));

	CCPoint p = pointFromDict("anchorPoint", props);
	node->setAnchorPoint(p);

	node->setRotation((float)atof(valueForKey("rotation", props)));

	node->setIsRelativeAnchorPoint(boolFromDict("isRelativeAnchorPoint", props));

	node->setIsVisible(boolFromDict("visible", props));

	node->setTag(atoi(valueForKey("tag", props)));
}

void XBReader::setPropForSprite(CCSprite* sprite, XBDictionary* props)
{
	const char* spriteFile = valueForKey("spriteFile", props);
	sprite->initWithFile(spriteFile);

#ifdef XBRICK
	// register sprite name
	Helper::getSingleton().setSpriteFileName(sprite, spriteFile);
#endif

	ccColor4B color = color4bFromDict("color", props);
	sprite->setColor(ccc3(color.r, color.g, color.b));
	sprite->setOpacity(color.a);

	sprite->setBlendFunc(blendFuncFromDict("blendFunc", props));

	sprite->setFlipX(boolFromDict("flipX", props));
	sprite->setFlipY(boolFromDict("flipY", props));
}

void XBReader::setPropForCCLayer(CCLayer* layer, XBDictionary* props)
{
	layer->setIsTouchEnabled(boolFromDict("touchEnabled", props));
	layer->setIsAccelerometerEnabled(boolFromDict("accelerometerEnabled", props));
	layer->setIsKeypadEnabled(boolFromDict("keypadEnabled", props));
}

void XBReader::setPropForCCLayerColor(CCLayerColor* layerColor, XBDictionary* props)
{
	ccColor4B color = color4bFromDict("color", props);
	layerColor->setColor(ccc3(color.r, color.g, color.b));
	layerColor->setOpacity(color.a);

	layerColor->setBlendFunc(blendFuncFromDict("blendFunc", props));
}

void XBReader::setPropForCCLayerGradient(CCLayerGradient* layerGradient, XBDictionary* props)
{
	layerGradient->initWithColor(color4bFromDict("color", props),
		color4bFromDict("endColor", props),
		pointFromDict("vector", props));
}

void XBReader::setPropForCCParticleSystem(CCParticleSystem* particleSystem, XBDictionary* props)
{
	particleSystem->initWithTotalParticles(atoi(valueForKey("totalParticles", props)));

	// setTexture must be called first
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage(valueForKey("textureFile", props)));

	particleSystem->setDuration(atof(valueForKey("duration", props)));

	int emitterMode = atoi(valueForKey("emitterMode", props));
	particleSystem->setEmitterMode(emitterMode);

	if (emitterMode == kCCParticleModeGravity)
	{
		particleSystem->setGravity(pointFromDict("gravity", props));
		particleSystem->setAngle(atof(valueForKey("angle", props)));
		particleSystem->setAngleVar(atof(valueForKey("angleVar", props)));
		particleSystem->setSpeed(atof(valueForKey("speed", props)));
		particleSystem->setSpeedVar(atof(valueForKey("speedVar", props)));
		particleSystem->setRadialAccel(atof(valueForKey("radialAccel", props)));
		particleSystem->setRadialAccelVar(atof(valueForKey("radialAccelVar", props)));
		particleSystem->setTangentialAccel(atof(valueForKey("tangentialAccel", props)));
		particleSystem->setTangentialAccelVar(atof(valueForKey("tangentialAccelVar", props)));
	} else
	{
		particleSystem->setStartRadius(atof(valueForKey("startRadius", props)));
		particleSystem->setStartRadiusVar(atof(valueForKey("startRadiusVar", props)));
		particleSystem->setEndRadius(atof(valueForKey("endRadius", props)));
		particleSystem->setEndRadiusVar(atof(valueForKey("endRadiusVar", props)));
		particleSystem->setRotatePerSecond(atof(valueForKey("rotatePerSecond", props)));
		particleSystem->setRotatePerSecondVar(atof(valueForKey("rotatePerSecondVar", props)));
	}

	particleSystem->setPosVar(pointFromDict("posVar", props));
	particleSystem->setLife(atof(valueForKey("life", props)));
	particleSystem->setLifeVar(atof(valueForKey("lifeVar", props)));

	particleSystem->setStartSpin(atof(valueForKey("startSpin", props)));
	particleSystem->setStartSpinVar(atof(valueForKey("startSpinVar", props)));
	particleSystem->setEndSpin(atof(valueForKey("endSpin", props)));
	particleSystem->setEndSpinVar(atof(valueForKey("endSpinVar", props)));

	particleSystem->setStartColor(color4fFromDict("startColor", props));
	particleSystem->setStartColorVar(color4fFromDict("startColorVar", props));
	particleSystem->setEndColor(color4fFromDict("endColor", props));
	particleSystem->setEndColorVar(color4fFromDict("endColorVar", props));

	particleSystem->setStartSize(atof(valueForKey("startSize", props)));
	particleSystem->setStartSizeVar(atof(valueForKey("startSizeVar", props)));
	particleSystem->setEndSize(atof(valueForKey("endSize", props)));
	particleSystem->setEndSizeVar(atof(valueForKey("endSizeVar", props)));

	particleSystem->setBlendFunc(blendFuncFromDict("blendFunc", props));

	particleSystem->setEmissionRate(atof(valueForKey("emissionRate", props)));
	particleSystem->setPositionType(kCCPositionTypeGrouped);
	particleSystem->setIsBlendAdditive(true);
}

void XBReader::setPropForBMFont(CCLabelBMFont* bmFont, XBDictionary* props)
{
	const char* str = valueForKey("string", props);
	const char* fontFile = valueForKey("fontFile", props);
	bmFont->initWithString(str, fontFile);

#ifdef XBRICK
	Helper::getSingleton().setFontFileName(bmFont, fontFile);
#endif
}

void XBReader::setPropForMenu(CCMenu* menu, XBDictionary* props)
{
	menu->initWithItems(NULL, NULL);
}

void XBReader::setPropForMenuItem(CCMenuItem* menuItem, XBDictionary* props)
{
	menuItem->setIsEnabled(boolFromDict("Enabled", props));
}

void XBReader::setPropForItemImage(CCMenuItemImage* itemImage, XBDictionary* props)
{
	const char* normalImage = valueForKey("spriteFileNormal", props);
	const char* disabledImage = valueForKey("spriteFileDisabled", props);
	const char* selectedImage = valueForKey("spriteFileSelected", props);

#ifdef XBRICK
	Helper::getSingleton().setItemImageFiles(itemImage, normalImage, selectedImage, disabledImage);
#endif

	// Please change last two parameters to yours
	itemImage->initFromNormalImage(normalImage, selectedImage, disabledImage, NULL, NULL);
}
