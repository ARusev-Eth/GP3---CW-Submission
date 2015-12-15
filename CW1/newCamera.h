//Includes
#include "GameConstants.h"

// Default values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;

class newCamera{


public:

	enum CameraMov{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	//Public Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//Euler
	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat Zoom;

	//Options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	cInputMgr *m_InputMgr;

	void newCamera::attachInputMgr(cInputMgr* inputMgr)
	{
		m_InputMgr = inputMgr;
	}

	//Constructor - Vectors (Default)
	newCamera::newCamera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat Yaw = YAW, GLfloat Pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
		this->Position = Position;
		this->WorldUp = WorldUp;
		this->Yaw = Yaw;
		this->Pitch = Pitch;
		this->updateCameraVectors();
	}

	//Constructor - Scalar values (Input)
	newCamera::newCamera(glm::vec3 Position, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat Yaw, GLfloat Pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		this->Position = Position;
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = Yaw;
		this->Pitch = Pitch;
		this->updateCameraVectors();
	}

	//Returns the view matrix @EAngles & @LookAt 
	glm::mat4 newCamera::GetViewMatrix(){
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	//Used for keyboard movement - accepting a predefined (enum) direction as input
	void newCamera::KeyboardMov(CameraMov direction, GLfloat elapsedTime){
		GLfloat velocity = this->MovementSpeed * elapsedTime;
		if (direction == FORWARD){
			this->Position += this->Front * velocity;
		}
		else if (direction == BACKWARD){
			this->Position -= this->Front * velocity;
		}
		else if (direction == LEFT){
			this->Position -= this->Right * velocity;
		}
		else if (direction == RIGHT){
			this->Position += this->Right * velocity;
		}
	}

	void newCamera::MouseMov(GLfloat xOff, GLfloat yOff, GLboolean constrainPitch = true){
		//Rotates relative to mouse sensitivity
		xOff *= this->MouseSensitivity;
		yOff *= this->MouseSensitivity;

		this->Yaw += xOff;
		this->Pitch += yOff;

		//Stop screen from flipping when rotating too far
		if (constrainPitch){
			if (this->Pitch > 89.0f){
				this->Pitch = 89.0f;
			}
			if (this->Pitch < -89.0f){
				this->Pitch = -89.0f;
			}
		}

		//Update vectors from updated angles
		this->updateCameraVectors();
	}

	void camFlags();
	void camMove();

private:
	//Update the front vector from the camera's updated euler angles
	void newCamera::updateCameraVectors(){

		//Update Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);

		//Update Right and Up vectors as well
		//Normalize to avoid speed fluctuation when looking looking far up or down
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};

