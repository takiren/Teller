#pragma once
#ifndef _TE_NODEEDITOR_H_
#define _TE_NODEEDITOR_H_
#define IMGUI_DEFINE_MATH_OPERATORS

#include<memory>
#include<string>
#include<map>
#include<vector>
#include"utility.h"
#include<cinder/CinderImGui.h>
#include<imgui_internal.h>
#include <utilities/builders.h>
#include"imgui_node_editor.h"
#include<utilities/widgets.h>
#include<filesystem>
#include"Tree.h"


//�m�[�h�G�f�B�^�p���N���X
namespace teller {

	namespace ed = ax::NodeEditor;
	namespace fs = std::filesystem;
	using namespace ax;
	namespace util = ax::NodeEditor::Utilities;
	using ax::Widgets::IconType;

	template<class NODE_TYPE, class SOCKET_TYPE>
	using TNodeManagerRef = std::unique_ptr<TNodeManager<NODE_TYPE, SOCKET_TYPE>>;

	template<class NODE_TYPE, class SOCKET_TYPE>
	class NodeEditorBase {
	private:
	protected:
		std::string									name_;
		int											s_PinIconSize = 24;

		ed::EditorContext*							gContext;
		TNodeManagerRef<NODE_TYPE, SOCKET_TYPE>		nodeManagerRef;

		utils::UIDGenerator							uidgen;
		//�ǉ��ł���m�[�h�̈ꗗ
		std::vector<TNodeSignature<SOCKET_TYPE>>	nodeSignatureVector;

		bool										bCreatingNewNode;

		void	DrawPinIcon(const std::shared_ptr<TSocketCore<NODE_TYPE, SOCKET_TYPE>> sckt, bool connected, int alpha);
		virtual ImColor GetIconColor(TSocketCore<NODE_TYPE, SOCKET_TYPE>& _type);

		//Opens a Popup to add Node.
		virtual void	OpenPopupAddNode();

		//It returns TNodeID
		TNodeID MakeNode(int _index);

		virtual void Initialize();

		virtual void DrawInputSocketInternal();

		virtual void DrawOutputSocketInternal();

		bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size);

	public:
		NodeEditorBase() = delete;
		NodeEditorBase(std::string _name) :
			name_(_name),
			uidgen(utils::UIDGenerator()),
			gContext(ed::CreateEditor()),
			bCreatingNewNode(false),
			nodeManagerRef(std::make_unique<TNodeManager<NODE_TYPE, SOCKET_TYPE>>())
		{
			Initialize();
		};
		virtual ~NodeEditorBase() = default;

		//�R�s�[�R���X�g���N�^�폜
		NodeEditorBase(const NodeEditorBase&) = delete;
		NodeEditorBase& operator=(const NodeEditorBase&) = delete;
		//���[�u����
		NodeEditorBase& operator=(NodeEditorBase&&) = default;

		virtual void LoadFile(fs::path _path);
		virtual void Tick();

		virtual void AddNodeSignature(TNodeSignature<SOCKET_TYPE> _nodeSig);
	};

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline ImColor teller::NodeEditorBase< NODE_TYPE, SOCKET_TYPE>::GetIconColor(TSocketCore<NODE_TYPE, SOCKET_TYPE>& socket)
	{
		switch (socket.icontype_)
		{
		case IconType::Flow:			return ImColor(255, 255, 255);
		case IconType::Circle:			return ImColor(220, 48, 48);
		case IconType::Square:			return ImColor(68, 201, 156);
		case IconType::Grid:			return ImColor(147, 226, 74);
		case IconType::RoundSquare:		return ImColor(255, 255, 255);
		case IconType::Diamond:			return ImColor(125, 125, 125);
		default:						return ImColor(0, 0, 0);
		}
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::NodeEditorBase< NODE_TYPE, SOCKET_TYPE>::OpenPopupAddNode()
	{
		std::string popname = name_;
		popname += "AddNode";

		ed::Suspend();
		if (ed::ShowBackgroundContextMenu()) {
			ImGui::OpenPopup(popname.c_str());
		}
		ed::Resume();

		ed::Suspend();
		auto currentNode = TNodeID{ 0 };

		auto nodeCreated = false;
		//TODO:�Ȃ����}�E�X�̈ʒu�ɏo�Ă���Ȃ�
		if (ImGui::BeginPopup(popname.c_str())) {

			ImGui::Text("Node list.");
			ImGui::Separator();

			int i = 0;
			auto newnodeposition = ImGui::GetMousePos();
			for (auto& nodesig : nodeSignatureVector)
				if (ImGui::Selectable(nodesig.name.c_str()))
				{
					currentNode = MakeNode(i);
					nodeCreated = true;
				}

			++i;

			ImGui::EndPopup();
		}
		ed::Resume();

		//NOTE:

		if (nodeCreated)
			ed::SetNodePosition(currentNode, ImGui::GetMousePos());
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::MakeNode(int _index)
	{
		auto nid = nodeManagerRef->AddNodeFromSignature(nodeSignatureVector[_index]);

		return nid;
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::Initialize()
	{
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::DrawInputSocketInternal()
	{

	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::DrawOutputSocketInternal()
	{
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline bool NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGuiID id = window->GetID("##Splitter");
		ImRect bb;
		bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
		bb.Max = bb.Min + ImGui::CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
		return ImGui::SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::DrawPinIcon(const std::shared_ptr<TSocketCore<NODE_TYPE, SOCKET_TYPE>> sckt, bool connected, int alpha)
	{
		ImColor  color = GetIconColor(*sckt.get());
		color.Value.w = alpha / 255.0f;

		/*
		switch (sckt->type_)
		{
		case teller::Socket_Data_Type::FLOW:		iconType = IconType::Flow;   break;
		case teller::Socket_Data_Type::BOOL:		iconType = IconType::Circle; break;
		case teller::Socket_Data_Type::INT:			iconType = IconType::Circle; break;
		case teller::Socket_Data_Type::OPTION:		iconType = IconType::Circle; break;
		case teller::Socket_Data_Type::STRING:		iconType = IconType::Circle; break;
		default:
			return;
		}
		*/

		ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), sckt->icontype_, connected, color, ImColor(32, 32, 32, alpha));
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::Tick()
	{
		ed::SetCurrentEditor(this->gContext);
		ed::Begin(name_.c_str());

		OpenPopupAddNode();

		//1.�m�[�h�}�l�[�W���[����ǂݎ���ĕ`��
		{
			typename util::BlueprintNodeBuilder builder;
			// �m�[�h�ŃC�e���[�V����
			for (auto& node : nodeManagerRef->nodes) {
				builder.Begin(node.second->ID_);
				//builder�ł̑���B
				{
					builder.Header(ImColor(255, 255, 255));
					{
						ImGui::Spring(0);
						ImGui::TextUnformatted(node.second->name_.c_str());
						ImGui::Spring(1);
						ImGui::Dummy(ImVec2(0, 28));
						ImGui::Spring(0);
					}
					builder.EndHeader();

					// �C���v�b�g�\�P�b�g�̕`��
					{
						auto alpha = ImGui::GetStyle().Alpha;
						for (auto& e : node.second->socketsInput) {
							builder.Input(e.first);
							ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
							DrawPinIcon(e.second, false, (int)(alpha * 255));

							DrawInputSocketInternal();

							ImGui::Spring(0);
							ImGui::Spring(0);
							ImGui::PopStyleVar();
							builder.EndInput();
						}
					}

					// �A�E�g�v�b�g�\�P�b�g�̕`��
					{
						auto alpha = ImGui::GetStyle().Alpha;
						for (auto& e : node.second->socketsOutput) {
							ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
							builder.Output(e.first);
							ImGui::Spring(0);
							ImGui::TextUnformatted(node.second->name_.c_str());
							ImGui::Spring(0);

							DrawOutputSocketInternal();

							DrawPinIcon(e.second, false, (int)(alpha * 255));
							ImGui::PopStyleVar();
							builder.EndOutput();
						}
					}
				}
				builder.End();
			}
		}
		//2.�����N�`��
		{
			auto links = nodeManagerRef->GetLinks();
			for (auto& link : links)
				ed::Link(link.ID_, link.InputID_, link.OutputID_);
		}

		//3.�����N����
		{
			//�m�[�h�𐶐����Ă��Ȃ��Ƃ��ɏ���
			if (!bCreatingNewNode) {
				if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f)) {
					auto showLabel = [](const char* label, ImColor _color) {
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
						auto size = ImGui::CalcTextSize(label);

						auto padding = ImGui::GetStyle().FramePadding;
						auto spacing = ImGui::GetStyle().ItemSpacing;

						ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

						auto rectMin = ImGui::GetCursorScreenPos() - padding;
						auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

						auto drawlist = ImGui::GetWindowDrawList();
						drawlist->AddRectFilled(rectMin, rectMax, _color, size.y * 0.15f);

						ImGui::TextUnformatted(label);
					};

					ed::PinId startPinId = 0, endPinId = 0;

					//NOTE:�l�X�g���[�����܂��񂩁H
					if (ed::QueryNewLink(&startPinId, &endPinId))
						if (startPinId && endPinId) 

							if (startPinId == endPinId)
							{
								ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
							}
							else
							{
								showLabel("+ Create Link", ImColor(32, 45, 32, 180));
								if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
								{
									nodeManagerRef->MakeLink<ed::PinId>(startPinId, endPinId);
								}
							}
						
				}
				ed::EndCreate();
			}
		}

		ed::End();
		ed::SetCurrentEditor(nullptr);
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::LoadFile(fs::path _path)
	{
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline void NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::AddNodeSignature(TNodeSignature<SOCKET_TYPE> _nodeSig)
	{
		nodeSignatureVector.push_back(_nodeSig);
	}
}

#endif // !_TE_NODEEDITOR_H_
