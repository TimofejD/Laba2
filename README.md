# Laboratory work 2
Here we initialize new class *Pipeline* that gets all transformations with matrices and apply it all to one matrix
```cpp
class Pipeline {...}
```
### In *private* we initialize class properties:

*m_scale* - vector for scaling our figure

*m_worldPos* - vector for moving our figure

*m_rotateInfo* - vector for rotating our figure

*m_transformation* - final matrix of transformations

*camera_move* - matrix that is responsible for camera movement

*camera_rotate* - matrix that is responsible for camera rotation

*m_persProj* - properties of projection matrix

*FOV* - vertical viewing height

*Width* - width of the screen

*Height* - height of the screen

*zNear* - near value of z (ex. [**10**;20])

*zFar* - fat value of z (ex. [10;**20**])

*m_camera* - properties of camera matrix

*Pos* - vector that is responsible for camera starting location

*Target* - vector that is responsible for camera direction

*Up* - vector that is perpendicular to the top
```cpp
private:
	glm::vec3 m_scale; 
	glm::vec3 m_worldPos;
	glm::vec3 m_rotateInfo;
	glm::mat4x4 m_transformation;
	glm::mat4x4 camera_move; 
	glm::mat4x4 camera_rotate;
	struct {
		float FOV;
		float Width;
		float Height;
		float zNear;
		float zFar;
	} m_persProj;
	struct {
		glm::vec3 Pos;
		glm::vec3 Target;
		glm::vec3 Up;
	} m_camera;
};
```
### In *public* we initialize the default constructor and create methods
*Scale, WorldPos, Rotate* - get and set custom coordinates to be used in matrix

*SetCamera* - get and set custom vector to be used in camera matrix
```cpp
Pipeline()
	{
		m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void Scale(float ScaleX, float ScaleY, float ScaleZ)
	{
		m_scale.x = ScaleX;
		m_scale.y = ScaleY;
		m_scale.z = ScaleZ;
	}

	void WorldPos(float PosX, float PosY, float PosZ)
	{
		m_worldPos.x = PosX;
		m_worldPos.y = PosY;
		m_worldPos.z = PosZ;
	}

	void Rotate(float RotateX, float RotateY, float RotateZ)
	{
		m_rotateInfo.x = RotateX;
		m_rotateInfo.y = RotateY;
		m_rotateInfo.z = RotateZ;
	}
	void SetCamera(glm::vec3& Pos, glm::vec3& Target, glm::vec3& Up)
	{
		m_camera.Pos = Pos;
		m_camera.Target = Target;
		m_camera.Up = Up;
	}
```
### Here we create methods that returns Projection matrix and set the parameters
*InitPerspectiveProj()* - function that creates and returns the Projection matrix

*ar* - aspect ratio

*zRange* - distance between zNear and zFar

*tanHalfFOV* - tangent of a half vertical viewing height for projection
```cpp
glm::mat4x4 InitPerspectiveProj() const
	{
		const float ar = m_persProj.Width / m_persProj.Height;
		const float zNear = m_persProj.zNear;
		const float zFar = m_persProj.zFar;
		const float zRange = zNear - zFar;
		const float tanHalfFOV = tanf(glm::radians(m_persProj.FOV / 2.0));
		glm::mat4x4 ProjMat(
			1.0f / (tanHalfFOV * ar), 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f,
			0.0f, 0.0f, (-zNear - zFar) / zRange, 2.0f * zFar * zNear / zRange,
			0.0f, 0.0f, 1.0f, 0.0f);
		return(ProjMat);
	}
	void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
	{
		m_persProj.FOV = FOV;
		m_persProj.Width = Width;
		m_persProj.Height = Height;
		m_persProj.zNear = zNear;
		m_persProj.zFar = zFar;
	}
```
### Here we create methods for normalizing vectors, multiplications vectors, creating and returning transforming and moving camera matrices 
*Cross()* - method that gets 2 vectors, multiplicate them and returnes new vector that is perpendicular to plane defined by the original vectors

*Normalize()* - method that normalizes vectors (makes the unit vector)

*InitCameraTransform()* - gets the target and up vectors, normalizes them, multiplies normalized vector with the old one and then these
2 multiplications multiplicates again to get the V vector. Then sets all vectors coordinates in the matrix and returnes it

*N* - vector from the camera to its target (Z axis)

*U* - vector that exits the camera to the right (X axis)

*V* - vector which come from the head to the sky (Y axis)

*InitCameraTranslation* - sets the coordinates to the matrix that moves the camera and returnes it
```cpp
  glm::vec3 Cross(glm::vec3 v, glm::vec3 u) {
		float x = v[1] * u[2] - v[2] * u[1];
		float y = v[2] * u[0] - v[0] * u[2];
		float z = v[0] * u[1] - v[1] * u[0];
		glm::vec3 crossed = { x, y, z };
		return (crossed);
	}
	glm::vec3 Normalize(glm::vec3 v) {
		float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
		v.x /= len;
		v.y /= len;
		v.z /= len;
		glm::vec3 normalized = { v.x, v.y, v.z };
		return (normalized);
	}
	glm::mat4x4 InitCameraTransform(const glm::vec3& Target, const glm::vec3& Up)
	{
		glm::vec3 N = Target;
		N = Normalize(N);
		glm::vec3 U = Up;
		U = Normalize(U);
		U = Cross(U,Target);
		glm::vec3 V = Cross(N,U);
		glm::mat4x4 CameraRotate(
			U.x, U.y, U.z, 0.0f,
			V.x, V.y, V.z, 0.0f,
			N.x, N.y, N.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		return (CameraRotate);
	}
	glm::mat4x4 InitCameraTranslation(float x, float y, float z)
	{
		glm::mat4x4 cammov(
			1.0f, 0.0f, 0.0f, -x,
			0.0f, 1.0f, 0.0f, -y,
			0.0f, 0.0f, 1.0f, -z,
			0.0f, 0.0f, 0.0f, 1.0f);
		return (cammov);
	}
```
### Also we have method that creates the rotation matrix
*RotMat()* - gets the coordinates and sets them to the corresponding matrix and then we return the rotation matrix

*x, y, z* - coordinates that we get back with asin function and set to the matrices of rotation
```cpp
glm::mat4x4 RotMat(float RotateX, float RotateY, float RotateZ)
{
	float x = asin(RotateX);
	float y = asin(RotateY);
	float z = asin(RotateZ);

	glm::mat4x4 rx(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(x), -sinf(x), 0.0f,
		0.0f, sinf(x), cosf(x), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4x4 ry(cosf(y), 0.0f, -sinf(x), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinf(x), 0.0f, cosf(y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4x4 rz(cosf(z), -sinf(z), 0.0f, 0.0f,
		sinf(z), cosf(z), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return(rz * ry * rx);
}
```
### Some additions in RenderSceneCB()
*Pipeline p* - creating a Pipeline type object

*p.Scale()* - set parameters that change our transformation matrix to scale our figure

*p.Worldpos()* - set parameters that change our transformation matrix to move our figure

*p.Rotate()* - set parameters that change our transformation matrix to rotate our figure

*p.SetPerspectiveProj(10.0f, WINDOW_WIDTH, WINDOW_HEIGHT, -100.0f, 100.0f)* - set parameters that change our transformation matrix to set the perspective projection.
**10.0f** - FOV, **WINDOW_WIDTH** - width of the screen, **WINDOW_HEIGHT** - height of the screen, **-100.0f** - zNear, **100.0f** -zFar.

*CameraPos()* - set parameters to the vector  to set the camera position

*CameraTarget()* - set parameters to the vector  to set the camera direction

*CameraUp()* - set parameters to the vector  to set the camera up

*p.SetCamera()* - set parameters that change our transformation matrix to setup our camera

*glLoadMatrixf(reinterpret_cast<const float*>(p.GetTrans()))* - replace the current matrix with the specified matrix. In our case it's **p.GetTrans()**

*p.GetTrans()* - returns transformation matrix
```cpp
Pipeline p;
	p.Scale(sinf(scale * 0.1f), sinf(scale * 0.1f), sinf(scale * 0.1f));
	p.WorldPos(0.0f, 0.0f, sinf(scale));
	p.Rotate(sinf(scale), sinf(scale), 0.0f);
	p.SetPerspectiveProj(10.0f, WINDOW_WIDTH, WINDOW_HEIGHT, -100.0f, 100.0f);
	
	glm::vec3 CameraPos(1.0f, 1.0f, 30.0f);
	glm::vec3 CameraTarget(-0.35f, 0.0f, 1.0f);
	glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
	p.SetCamera(CameraPos, CameraTarget, CameraUp);

	glLoadMatrixf(reinterpret_cast<const float*>(p.GetTrans()));
```
### Method that create transformation matrix and returns it
*GetTrans()* - create and returns transformation matrix

*TranslationTrans* - matrix that is responsible for moving our figure

*RotateTrans* - matrix that is responsible for rotating our figure

*RotMat()* - method that gets the parameters (coordinates) and returns the rotation matrix

*ScaleTrans* - matrix that is responsible for scaling our figure

*ProjectionMatrix* -  matrix that is responsible for perspective projection

*InitPerspectiveProj()* - method that creates and returns the perspective projection matrix

*CamRotation* - matrix that is responsible for camera rotation

*InitCameraTransform* - method that gets the parameters (direction and up) and returns matrix of camera rotation

*CamMove* - matrix that is responsible for camera postion

*InitCameraTranslation()* - method that gets the parameters (coordinates of position) and returns matrix of camera position

*m_transformation* - transformation matrix that is a product of all previous matrices
```cpp
const glm::mat4x4* GetTrans()
	{
		glm::mat4x4 TranslationTrans(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			m_worldPos.x, m_worldPos.y, m_worldPos.z, 1.0f);
		glm::mat4x4 RotateTrans = RotMat(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
		glm::mat4x4 ScaleTrans(
			m_scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, m_scale.x, 0.0f, 0.0f,
			0.0f, 0.0f, m_scale.x, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4x4 ProjectionMatrix = InitPerspectiveProj();
		glm::mat4x4 CamRotation = InitCameraTransform(m_camera.Target, m_camera.Up);
		glm::mat4x4 CamMove = InitCameraTranslation(m_camera.Pos.x, m_camera.Pos.y, m_camera.Pos.z);

		m_transformation = ProjectionMatrix * CamRotation * CamMove * TranslationTrans * RotateTrans * ScaleTrans;
		return &m_transformation;
	}
```
