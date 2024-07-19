#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>

using namespace std;

// Overload << operator for glm::mat4
//OpenGL developers often use an internal matrix layout called column-major ordering which is the default matrix layout in GLM
std::ostream& operator<<(std::ostream& os, const glm::mat4& matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            os << matrix[j][i] << ' ';
        }
        os << '\n';
    }
    return os;
}

// Overload << operator for glm::vec4
std::ostream& operator<<(std::ostream& os, const glm::vec4& vector) {
    os << vector.x << ' ' << vector.y << ' ' << vector.z << ' ' << vector.w;
    return os;
}


int main07190324()
{
    // Initialize a glm::mat4 with specific values
    glm::mat4 matrix1(
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 0.0f,
        13.0f, 14.0f, 1.0f, 0.0f
    );

    // Print the matrix
    std::cout << "Transformation Matrix:" << std::endl;
    std::cout << matrix1 << std::endl;


	//translation 
    glm::vec4 vec;
	glm::vec4 vec1(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	cout << trans << endl;
	vec = trans * vec1;
	std::cout << vec.x << vec.y << vec.z << std::endl;
    cout << vec << endl;

    //scale and rotate
    cout << "------------------------scale and rotate-----------------" << endl;
    glm::mat4 transscalerotate = glm::mat4(1.0f);
    //rotate the container 90 degrees around the Z - axis.GLM expects its angles in radians so we convert the degrees to radians using glm::radians
    transscalerotate = glm::rotate(transscalerotate, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //cout << transscalerotate << endl;
    transscalerotate = glm::scale(transscalerotate, glm::vec3(0.5, 0.5, 0.5));
    cout << transscalerotate << endl;

    return 0;
}
