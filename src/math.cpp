#include <util/math.hpp>
#include <component/camera.hpp>
#include <iostream>

glm::mat4 Math::CreateTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, float scale)
{
    glm::mat4 matrix(1.0f);

    matrix = glm::translate(matrix, translation);
    matrix = glm::rotate(matrix,glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
    matrix = glm::rotate(matrix,glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
    matrix = glm::rotate(matrix,glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
    matrix = glm::scale(matrix, glm::vec3(scale, scale, scale)); /* Uniformly scale in all direction */

    return matrix;
}

glm::mat4 Math::CreateProjectionMatrix(ProjectionDetails* projectionDetails)
{
    float aspectRatio = static_cast<float>(projectionDetails->_width) / static_cast<float>(projectionDetails->_height);
	glm::mat4 pMatrix = glm::perspective(glm::radians(projectionDetails->_fov), aspectRatio, projectionDetails->_zNear, projectionDetails->_zFar);

    return pMatrix;
}

glm::mat4 Math::CreateViewMatrix(Camera* camera)
{
    glm::mat4 viewMatrix = glm::lookAt(
		camera->GetPositionVector(),
        camera->GetPositionVector() + camera->GetFrontVector(),
        camera->GetUpVector()
	);

    // glm::mat4 viewMatrix = glm::lookAt(
	// 	glm::vec3(0,0,3), 	    // Camera is at (4,3,3), in World Space
	//  glm::vec3(0,0,0), 		// and looks at the origin
	// 	glm::vec3(0,1,0)  		// Head is up (set to 0,-1,0 to look upside-down)
	// );

    return viewMatrix;
}
