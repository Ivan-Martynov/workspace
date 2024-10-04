#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <vector>

#include "Game_object.h"
#include "Sprite_renderer.h"
#include "Resource_manager.h"

class Game_level {
public:
    Game_level(): bricks_ {} { };

    explicit Game_level(const Game_level&) = default;
    Game_level& operator=(const Game_level&) = default;
    explicit Game_level(Game_level&&) = default;
    Game_level& operator=(Game_level&&) = default;

    ~Game_level() = default;

    void load(const std::string&, const GLuint, const GLuint);
    void draw(Sprite_renderer& rend);
    GLboolean is_completed();

public:
    std::vector<Game_object> bricks_;
private:
    void init(const std::vector<std::vector<GLuint>>&, const GLuint,
            const GLuint);
};

void Game_level::load(const std::string& lvl_fn, const GLuint lvl_width,
        const GLuint lvl_height) {
    // clear old level data
    bricks_.clear();
    //Game_level level;

    using namespace std;

    // initialize data for new bricks
    vector<vector<GLuint>> tile_data;

    // reading a file with level data
    ifstream ifs {lvl_fn};
    if (!ifs)
        throw runtime_error {"cannot open file " + string(lvl_fn)};
    ifs.exceptions(ifs.exceptions() | ios_base::badbit);
    // reading lines from a level file
    for (string s; getline(ifs, s);) {
        if (ifs.fail()) {
            ifs.unget();
            ifs.clear(ios_base::failbit);
        }
        // reading numbers from a string
        istringstream ss {s};
        vector<GLuint> row;
        for (GLuint i; ss >> i;) {
            row.push_back(i);
            if (ss.fail()) {
                ss.unget();
                ss.clear(ios_base::failbit);
            }
        }
        // assemble rows in the bricks data
        tile_data.push_back(row);
    }
    // initialize a game level
    if (tile_data.size())
        init(tile_data, lvl_width, lvl_height);
}

void Game_level::init(const std::vector<std::vector<GLuint>>& tile_data,
        const GLuint lvl_width, const GLuint lvl_height) {
    // calculate size of a block (brick)
    const GLuint height_val = tile_data.size(), width_val = tile_data[0].size();
    const GLfloat unit_width = lvl_width / static_cast<GLfloat>(width_val);
    const GLfloat unit_height = lvl_height / height_val;

    // initialize level tiles
    for (GLuint y {0}; y < height_val; ++y) {
        for (GLuint x {0}; x < width_val; ++x) {
            // check block type
            if (tile_data[y][x] == 1) { // solid block
                const glm::vec2 unit_pos {unit_width * x, unit_height * y};
                const glm::vec2 unit_size {unit_width, unit_height};
                Game_object game_obj {unit_pos, unit_size,
                    Resource_manager::texture("block_solid"),
                    glm::vec3{0.8, 0.8, 0.7}};
                game_obj.is_solid_ = GL_TRUE;
                bricks_.push_back(game_obj);
            } else if (tile_data[y][x] > 1) {
                glm::vec3 clr {1}; // white color
                switch (tile_data[y][x]) {
                    case 2:
                        clr = glm::vec3{0.2, 0.6, 1};
                        break;
                    case 3:
                        clr = glm::vec3{0, 0.7, 0};
                        break;
                    case 4:
                        clr = glm::vec3{0.8, 0.8, 0.4};
                        break;
                    case 5:
                        clr = glm::vec3{1, 0.5, 0};
                        break;
                    default:
                        break;
                }
                const glm::vec2 unit_pos {unit_width * x, unit_height * y};
                const glm::vec2 unit_size {unit_width, unit_height};
                bricks_.push_back(Game_object(unit_pos, unit_size,
                            Resource_manager::texture("block"), clr));
            }
        }
    }
}

void Game_level::draw(Sprite_renderer& rend) {
    for (Game_object& tile: bricks_)
        if (!tile.is_destroyed_)
            tile.draw(rend);
}

GLboolean Game_level::is_completed() {
    for (Game_object& tile: bricks_)
        if (!tile.is_solid_ && !tile.is_destroyed_)
            return GL_FALSE;
    return GL_TRUE;
}


#endif /* GAME_LEVEL_H */

