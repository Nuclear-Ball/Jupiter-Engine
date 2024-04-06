#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include <pos2d.h>
#include <shader.h>

namespace JWrapp {


	void draw_line(pos2d start, pos2d end, int thickness, Shader *sh) {
		sh->Use();
		const int ct_x = (end.x - start.x), ct_y = (end.y - start.y);
		const float l = sqrt(pow(ct_x, 2) + pow(ct_y, 2));
		const int q_ids[] = { 0, 1, 3, 2 };
		const int quarter = q_ids[(((ct_y < 0) << 1) + (ct_x < 0))];
		const double pi = 3.14159265358979323846;
		const float rotation = (asinf(static_cast<float>(ct_x) / l)) * (ct_y >= 0 ? -1 : 1) + (pi / 2) + (ct_y >= 0 ? 0 : pi);

		model = glm::translate(model, glm::vec3(static_cast<float>(start.x), -(static_cast<float>(start.y)), 0.0f));
		model = glm::rotate(model, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3((l + thickness), static_cast<float>(thickness), 1.0f));
		flat_sh.setMat4("model", model);
		model = glm::mat4(1);
		flat_sh.setVec4("flat_color", glm::vec4(1.0, 1.0, 1.0, 1.0));
		glBindVertexArray(VAO.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		txt_sh.Use();
	}
}