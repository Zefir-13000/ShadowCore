#pragma once
#include <Core/Core.h>

using namespace SC;

namespace SC::Editor {
	static std::shared_ptr<RenderTexture> viewport = nullptr;
	static std::shared_ptr<PickingTexture> pickingTexture = nullptr;
	static std::shared_ptr<Shader> pickingShader = nullptr;
	static uint32_t selected_ObjectID = 0, prevSelected_ObjectID = 0;
	static std::shared_ptr<Object> selected_Object = nullptr;
	static glm::vec3 start_objectDrag(0.f);
	static char objectNameBuffer[MAXLEN_OBJECT_NAME];
	static bool objectNameEdited = false;

	// move arrow gizmo
	static std::shared_ptr<Arrow> arrow_x = nullptr, arrow_y = nullptr, arrow_z = nullptr, selected_arrow = nullptr;
	static bool arrow_dragged = false;

	void PickingPhase();
	void EditorObjectsInit();

	static struct MouseLocalPos {
		int x, y;
	} MousePos;
}