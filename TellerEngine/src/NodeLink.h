#include<imgui_node_editor.h>
#include <utilities/builders.h>

namespace ed = ax::NodeEditor;

struct LinkInfo {
	ed::LinkId Id;
	ed::PinId  InputId;
	ed::PinId  OutputId;
};