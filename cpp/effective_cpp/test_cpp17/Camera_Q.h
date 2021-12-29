#ifndef CAMERA_Q_H
#define CAMERA_Q_H

class Camera_q {
	glm::vec3 m_pos = glm::vec3(0, 0, 0);
	glm::quat m_orient = glm::quat();
public:
	Camera_q(void) = default;
	Camera_q(const Camera_q &) = default;

	Camera_q(const glm::vec3 &pos) : m_pos(pos) {}
    Camera_q(const glm::vec3 &pos, const glm::quat &orient) :
        m_pos(pos), m_orient(orient) {}

	Camera_q &operator =(const Camera_q &) = default;

	const glm::vec3 &position() const { return m_pos; }
	const glm::quat &orientation() const { return m_orient; }

    glm::mat4 view() const {
        return glm::translate(glm::mat4_cast(m_orient), m_pos);
    }

	void translate(const glm::vec3 &v) { m_pos += v * m_orient; }
	void translate(float x, float y, float z) {
        m_pos += glm::vec3(x, y, z) * m_orient;
    }

	void rotate(float angle, const glm::vec3 &axis) {
        m_orient *= glm::angleAxis(angle, axis * m_orient);
    }
	void rotate(float angle, float x, float y, float z) {
        m_orient *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orient);
    }

    void process_mouse_move(const GLfloat, const GLfloat);

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
};

void Camera_q::process_mouse_move(const GLfloat x_offset, const GLfloat y_offset)
{
    constexpr auto a = 0.8 * std::acos(-1) / 180;
    if (x_offset > 0)
        yaw(a);
    else
        yaw(-a);

    if (y_offset < 0)
        pitch(a);
    else
        pitch(-a);
    //pitch(y_offset);
    // update Euler angles
    //yaw_ang_   += (x_offset * mouse_sensitivity_);
    //pitch_ang_ += (y_offset * mouse_sensitivity_);
}


#endif /* CAMERA_Q_H */
