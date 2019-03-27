#pragma once

#include "ofMain.h"
#include <openvr.h>
#include "CGLRenderModel.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
enum class ControllerRole
{
	Left = 0,
	Right = 1,
	Unknown = 3
};

//--------------------------------------------------------------
enum class EventType
{
	ButtonPress = 0,
	ButtonUnpress = 1,
	ButtonTouch = 2,
	ButtonUntouch = 3
};

//--------------------------------------------------------------
enum class ButtonType
{
	ButtonSystem = 0,
	ButtonApplicationMenu = 1,
	ButtonGrip = 2,
	ButtonTouchpad = 3,
	ButtonTrigger = 4
};

//--------------------------------------------------------------
class ofxOpenVRControllerEventArgs : public ofEventArgs
{
public:
	ControllerRole controllerRole;
	ButtonType buttonType;
	EventType eventType;
	float analogInput_xAxis;
	float analogInput_yAxis;
};
//--------------------------------------------------------------
class ofxOpenVR {

public:
	void setup(std::function< void(vr::Hmd_Eye) > f);
	void exit();

	void update();
	void render();
	void renderDistortion(int x, int y, float width, float height);

	void drawDebugInfo(float x = 10.0f, float y = 20.0f);

	glm::mat4x4 getHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	glm::mat4x4 getHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	glm::mat4x4 getHMDMatrixPoseHMD();

	glm::mat4x4 getCurrentViewProjectionMatrix(vr::Hmd_Eye nEye);
	glm::mat4x4 getCurrentProjectionMatrix(vr::Hmd_Eye nEye);
	glm::mat4x4 getCurrentViewMatrix(vr::Hmd_Eye nEye);

	glm::mat4x4 getControllerPose(vr::ETrackedControllerRole nController);
	glm::mat4x4 getTrackerPose();
	bool isControllerConnected(vr::ETrackedControllerRole nController);

	void setDrawControllers(bool bDrawControllers);
	void setClearColor(ofFloatColor color);

	void showMirrorWindow();
	void hideMirrorWindow();
	void toggleMirrorWindow();

	void setRenderModelForTrackedDevices(bool bRender);
	bool getRenderModelForTrackedDevices();

	void toggleGrid(float transitionDuration = 2.0f);
	void showGrid(float transitionDuration = 2.0f);
	void hideGrid(float transitionDuration = 2.0f);

	void sendHaptics(int controllerIndex, int axis, unsigned short durationMicros);
	void longVibration(float length, float strength);
	ofEvent<ofxOpenVRControllerEventArgs> ofxOpenVRControllerEvent;
	glm::mat4x4 getmat4HMDPose();

	bool connected();
	bool _bUseCamera;

private:

	struct VertexDataScene
	{
		glm::vec3 position;
		glm::vec2 texCoord;
	};

	struct VertexDataLens
	{
		glm::vec2 position;
		glm::vec2 texCoordRed;
		glm::vec2 texCoordGreen;
		glm::vec2 texCoordBlue;
	};

	struct FramebufferDesc
	{
		GLuint _nDepthBufferId;
		GLuint _nRenderTextureId;
		GLuint _nRenderFramebufferId;
		GLuint _nResolveTextureId;
		GLuint _nResolveFramebufferId;
	};
	FramebufferDesc leftEyeDesc;
	FramebufferDesc rightEyeDesc;

	std::function< void(vr::Hmd_Eye) > _callableRenderFunction;

	bool _bGlFinishHack;
	bool _bIsGLInit;
	bool _bIsGridVisible;
	
	ofFloatColor _clearColor;

	uint32_t _nRenderWidth;
	uint32_t _nRenderHeight;

	float _fNearClip;
	float _fFarClip;

	vr::IVRSystem *_pHMD;

	vr::IVRRenderModels *_pRenderModels;
	std::string _strTrackingSystemName;
	std::string _strTrackingSystemModelNumber;
	vr::TrackedDevicePose_t _rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	glm::mat4x4 _rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];

	vr::IVRTrackedCamera *_pTrackedCamera;
	vr::TrackedDeviceIndex_t _trackedCameraIndex;
	vr::TrackedCameraHandle_t _trackedCameraHandle;
	uint32_t _nTrackedCameraFrameWidth, _nTrackedCameraFrameHeight, _nTrackedCameraFrameSize;
	ofPixels _trackedCameraPix;
	vr::HmdVector2_t _trackedCameraFocalLength;
	vr::HmdVector2_t _trackedCameraCenter;
	vr::HmdMatrix44_t _trackedCameraProjectionMatrix;

	int _iTrackedControllerCount;
	int _iTrackedControllerCount_Last;
	int _iValidPoseCount;
	int _iValidPoseCount_Last;

	std::ostringstream _strPoseClassesOSS;

	ofShader _lensShader;
	GLuint _unLensVAO;
	GLuint _glIDVertBuffer;
	GLuint _glIDIndexBuffer;
	unsigned int _uiIndexSize;

	glm::mat4x4 _genericTrackerPose;

	glm::mat4x4 _mat4HMDPose;
	glm::mat4x4 _mat4eyePosLeft;
	glm::mat4x4 _mat4eyePosRight;

	glm::mat4x4 _mat4ProjectionCenter;
	glm::mat4x4 _mat4ProjectionLeft;
	glm::mat4x4 _mat4ProjectionRight;

	int _leftControllerDeviceID;
	int _rightControllerDeviceID;
	glm::mat4x4 _mat4LeftControllerPose;
	glm::mat4x4 _mat4RightControllerPose;
	glm::mat4x4 _mat4GenericTrackerrPose;

	int _lastGenericTrackerID;
	glm::mat4x4 _mat4LastGenericTrackerPose;

	bool _bDrawControllers;
	ofVboMesh _controllersVbo;
	ofShader _controllersTransformShader;

	bool init();
	bool initGL();
	bool initCompositor();

	bool createAllShaders();
	bool createFrameBuffer(int nWidth, int nHeight, FramebufferDesc &framebufferDesc);

	bool setupStereoRenderTargets();
	void setupDistortion();
	void setupCameras();

	void updateDevicesMatrixPose();
	void updateTrackedCamera();

	void handleInput();
	void processVREvent(const vr::VREvent_t & event);

	void renderStereoTargets();
	
	void drawControllers();
	void renderScene(vr::Hmd_Eye nEye);

	ofPixels getTrackedCameraPix() { return _trackedCameraPix; };
	ofVec2f getTrackedCameraFocalLength() { return ofVec2f(_trackedCameraFocalLength.v[0], _trackedCameraFocalLength.v[1]); }
	ofVec2f getTrackedCameraCenter() { return ofVec2f(_trackedCameraCenter.v[0], _trackedCameraCenter.v[1]); }
	ofMatrix4x4 getTrackedCameraProjectionMatrix() { return ofMatrix4x4::getTransposedOf(ofMatrix4x4((float*)_trackedCameraProjectionMatrix.m)); }

	ofMatrix4x4 getLastGenericTrackerPose() { return ofMatrix4x4(&_mat4LastGenericTrackerPose[0][0]); }
	ofMatrix4x4 getHmdPose() { return ofMatrix4x4(&(glm::inverse(_mat4HMDPose))[0][0]); }
	ofMatrix4x4 getLeftEyePose() { return ofMatrix4x4(&(glm::inverse(_mat4HMDPose)*(glm::inverse(_mat4eyePosLeft)))[0][0]); }
	ofMatrix4x4 getRightEyePose() { return ofMatrix4x4(&(glm::inverse(_mat4HMDPose)*(glm::inverse(_mat4eyePosRight)))[0][0]); }

	glm::mat4x4 convertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose);
	
	bool _bRenderModelForTrackedDevices;
	ofShader _renderModelsShader;
	CGLRenderModel* findOrLoadRenderModel(const char *pchRenderModelName);
	void setupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t unTrackedDeviceIndex);
	void setupRenderModels();

	std::vector< CGLRenderModel * > _vecRenderModels;
	CGLRenderModel *_rTrackedDeviceToRenderModel[vr::k_unMaxTrackedDeviceCount];
};
