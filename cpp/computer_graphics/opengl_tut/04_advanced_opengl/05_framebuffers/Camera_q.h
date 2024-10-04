#ifndef CAMERA_Q_H
#define CAMERA_Q_H

class Camera_q {
private:
	glm::vec3 m_pos = glm::vec3(0, 0, 0);
	glm::quat m_orient = glm::quat();

    float key_yaw = 0;
    float key_pitch = 0;
    float key_roll = 0;

public:

    Camera_q() = default;
	Camera_q(const Camera_q &) = default;

	Camera_q(const glm::vec3 &pos) : m_pos(pos) {}
    Camera_q(const glm::vec3 &pos, const glm::quat &orient) :
        m_pos(pos), m_orient(orient) {}

	Camera_q &operator =(const Camera_q &) = default;

	const glm::vec3 &position() const { return m_pos; }
	const glm::quat &orientation() const { return m_orient; }

    glm::mat4 view_matrix() const {
        glm::normalize(m_orient);
        return glm::translate(glm::mat4_cast(m_orient), -m_pos);
    }

	void translate(const glm::vec3 &v) { m_pos += v * m_orient; }
	void translate(float x, float y, float z) {
        m_pos += glm::vec3(x, y, z) * m_orient;
    }

	void rotate(float angle, const glm::vec3 &axis) {
        m_orient *= glm::angleAxis(angle, axis * m_orient);
    }
	void rotate(float angle, float x, float y, float z) {
        //m_orient = m_orient + glm::angleAxis(angle, glm::vec3(x, y, z) *
        //        m_orient);
        //m_orient = glm::mix(m_orient, q, 0.5f);
        //const auto q = m_orient;
        //m_orient = glm::angleAxis(angle, glm::vec3(x, y, z));
        m_orient *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orient);
        //m_orient = glm::mix(q, m_orient, 0.5f);
    }

    void process_mouse_move(const GLfloat, const GLfloat);

	void yaw(float angle) {
        //std::cout << glm::yaw(m_orient) * 180 / std::acos(-1) << '\n';
        rotate(angle, 0.0f, 1.0f, 0.0f);
        //const auto q = m_orient;
        //m_orient = glm::mix(m_orient,
        //        glm::angleAxis(key_yaw += angle, glm::vec3(1, 0, 0)), 0.5f);
        //key_yaw += angle;
    }
	void pitch(float angle) {
        //std::cout << glm::pitch(m_orient) * 180 / std::acos(-1) << '\n';
        rotate(angle, 1.0f, 0.0f, 0.0f);
        //m_orient = glm::mix(m_orient,
        //        glm::angleAxis(key_pitch += angle, glm::vec3(1, 0, 0)), 0.5f);
        //key_pitch += angle;
    }
	void roll(float angle) {
        //std::cout << glm::roll(m_orient) * 180 / std::acos(-1) << '\n';
        rotate(angle, 0.0f, 0.0f, 1.0f);
        //m_orient = glm::mix(m_orient,
        //        glm::angleAxis(key_roll += angle, glm::vec3(1, 0, 0)), 0.5f);
        //key_roll += angle;
    }

    glm::mat4 update_view();

    glm::mat4 update_orientation() {
        const auto q = m_orient;
        m_orient = glm::angleAxis(key_pitch, glm::vec3(1, 0, 0));
        m_orient *= glm::angleAxis(key_yaw, glm::vec3(0, 1, 0));
        m_orient *= glm::angleAxis(key_roll, glm::vec3(0, 0, 1));

        m_orient = glm::mix(q, m_orient, 0.2f);
        return glm::translate(glm::mat4_cast(m_orient), -m_pos);
    }
};

void Camera_q::process_mouse_move(const GLfloat x_offset, const GLfloat y_offset)
{
    constexpr auto a = 2 * std::acos(-1) / 180;

    if (x_offset > 0)
        yaw(a);
    else
        yaw(-a);

    if (y_offset < 0)
        pitch(a);
    else
        pitch(-a);

    //if (x_offset > 0)
    //    key_yaw += a;
    //else
    //    key_yaw -= a;

    //if (y_offset > 0)
    //    key_pitch -= a;
    //else
    //    key_pitch += a;

    //if (key_yaw > std::acos(-1) * 2)
    //    key_yaw -= std::acos(-1) * 2;
    //else if (key_yaw < -std::acos(-1) * 2)
    //    key_yaw += std::acos(-1) * 2;
    //if (key_pitch > std::acos(-1) * 2)
    //    key_pitch -= std::acos(-1) * 2;
    //else if (key_pitch < -std::acos(-1) * 2)
    //    key_pitch += std::acos(-1) * 2;
}

glm::mat4 Camera_q::update_view() {
    //temporary frame quaternion from pitch,yaw,roll 
    //here roll is not used
    glm::quat key_quat = glm::quat(glm::vec3(key_pitch, key_yaw, 0));
    //reset values
    key_pitch = key_yaw = key_roll = 0;

    //order matters,update camera_quat
    m_orient = key_quat * m_orient;
    m_orient = glm::normalize(m_orient);
    glm::mat4 rotate = glm::mat4_cast(m_orient);

    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, -m_pos);

    return rotate * translate;
}

#endif /* CAMERA_Q_H */

