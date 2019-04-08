#include <glad/glad.h>
#include <vector>
#include <utilityfunctions.h>

/* this movement element is limited by enum to increase safety (mistakingly adding another movement that i don't want for example) */
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
	UP,
	DOWN
};

/* initial camera values to catch to similar angle shown in the assignment 5 sheet */
const float YAW         =  167.0f;
const float PITCH       = -48.3f;
const float SPEED       =  10.0f;
const float SENSITIVITY =  0.1f;


/* camera class */
class Camera
{
public:
    
    Vector3 Position;
	Vector3 Front;
	Vector3 Up;
	Vector3 Right;
	Vector3 WorldUp;
    
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;

	/* construct an object with corresponding values */
    Camera(Vector3 position = Hector3(0.0f, 0.0f, 0.0f), Vector3 up = Hector3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(Hector3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

	/* call the lookat function in utilityfunctions */
    float* lookAtFunc()
    {
        return myLookAtFunction(Position, vectorAdditionWithReturn(Position, Front), Up);
    }

    /* for WASD movement */
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
		//std::cout << " X = " << Position.x << " Y = " << Position.y << " Z = " << Position.z << std::endl;
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
			Position = vectorAdditionWithReturn(Position, vectorScalarMultiplication( Front , velocity)) ;
        if (direction == BACKWARD)
			Position = vectorSubtractionWithReturn(Position, vectorScalarMultiplication(Front, velocity));
        if (direction == LEFT)
			Position = vectorSubtractionWithReturn(Position, vectorScalarMultiplication(Right, velocity));
        if (direction == RIGHT)
			Position = vectorAdditionWithReturn(Position, vectorScalarMultiplication(Right, velocity));
		if (direction == UP)
			Position = vectorAdditionWithReturn(Position, vectorScalarMultiplication(Up, velocity));
		if (direction == DOWN)
			Position = vectorSubtractionWithReturn(Position, vectorScalarMultiplication(Up, velocity));

		//std::cout << "X= " << Position.x << "Y= " << Position.y << "Z= " << Position.z << std::endl;
    }

    /* for mouse movement */
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        /* takla atmasin diye */
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
		//std::cout << "  Yaw =    " << Yaw << "  Pitch =   " << Pitch << std::endl;
        updateCameraVectors();
    }


private:
    
    void updateCameraVectors()
    {
        /* calculate the new camera front when movement occurs*/
        Vector3 front;
        front.x = cos(toRadians*(Yaw)) * cos(toRadians*(Pitch));
		//std::cout << front.x << std::endl;
        front.y = sin(toRadians*(Pitch));
        front.z = sin(toRadians*(Yaw)) * cos(toRadians*(Pitch));
		//std::cout << " Yaw =   " << Yaw << "  Pitch = " << Pitch << std::endl;
        Front = vectorNormalization(front);
		//std::cout << vectorLength(Front) << std::endl;
		//std::cout << "X= " << Front.x << "Y= " << Front.y << "Z= " << Front.z << std::endl;
        Right = vectorNormalization(vectorCross(Front, WorldUp));  
        Up    = vectorNormalization(vectorCross(Right, Front));
		//std::cout << "X= " << Front.x << "Y= " << Front.y << "Z= " << Front.z << std::endl;
		//std::cout << " X = " << Front.x << " Y = " << Front.y << " Z = " << Front.z << std::endl;
    }
};