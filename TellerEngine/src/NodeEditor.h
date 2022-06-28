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


//ノードエディタ用基底クラス
namespace teller {

	namespace ed = ax::NodeEditor;
	namespace fs = std::filesystem;
	using namespace ax;
	namespace util = ax::NodeEditor::Utilities;
	using ax::Widgets::IconType;

	template<class NODE_TYPE, class SOCKET_TYPE>
	class NodeEditorBase {
	private:
	protected:
		std::string								name_;
		int										s_PinIconSize = 24;

		ed::EditorContext* gContext;
		std::unique_ptr<TNodeManager<NODE_TYPE, SOCKET_TYPE>>			TNodeManagerRef;

		utils::UIDGenerator						uidgen;
		//追加できるノードの一覧
		std::vector<TNodeSignature<SOCKET_TYPE>>				nodeSignatureVector;

		bool									bCreatingNewNode;

		void	DrawPinIcon(const std::shared_ptr<TSocketCore<NODE_TYPE, SOCKET_TYPE>> sckt, bool connected, int alpha);
		ImColor GetIconColor(TSocketCore<NODE_TYPE, SOCKET_TYPE>& _type);

		//Opens a Popup to add Node.
		void	OpenPopupAddNode();

		//It returns TNodeID
		TNodeID MakeNode(int _index);

		virtual void Initialize();

		virtual void DrawInputSocketInternal();

		virtual void DrawOutputSocketInternal();

	public:
		NodeEditorBase() = delete;
		NodeEditorBase(std::string _name) :
			name_(_name),
			uidgen(utils::UIDGenerator()),
			gContext(ed::CreateEditor()),
			bCreatingNewNode(false),
			TNodeManagerRef(std::make_unique<TNodeManager<NODE_TYPE, SOCKET_TYPE>>())
		{
			Initialize();
		};
		virtual ~NodeEditorBase() = default;

		//コピーコンストラクタ削除
		NodeEditorBase(const NodeEditorBase&) = delete;
		NodeEditorBase& operator=(const NodeEditorBase&) = delete;
		//ムーブ許可
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

		if (ImGui::BeginPopup(popname.c_str())) {

			ImGui::Text("Node list.");
			ImGui::Separator();

			int i = 0;

			for (auto& nodesig : nodeSignatureVector)
				if (ImGui::Selectable(nodesig.name.c_str()))
					ed::SetNodePosition(MakeNode(i), ImGui::GetMousePos());

			++i;

			ImGui::EndPopup();
		}
		ed::Resume();
	}

	template<class NODE_TYPE, class SOCKET_TYPE>
	inline TNodeID teller::NodeEditorBase<NODE_TYPE, SOCKET_TYPE>::MakeNode(int _index)
	{
		auto nid = TNodeManagerRef->AddNodeFromSignature(nodeSignatureVector[_index]);

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

		//1.ノードマネージャーから読み取って描画
		{
			typename util::BlueprintNodeBuilder builder;
			// ノードでイテレーション
			for (auto& node : TNodeManagerRef->nodes) {
				builder.Begin(node.second->ID_);
				//builderでの操作。
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

					// インプットソケットの描画
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

					// アウトプットソケットの描画
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
		//2.リンク描画
		{
			auto links = TNodeManagerRef->GetLinks();
			for (auto& link : links)
				ed::Link(link.ID_, link.InputID_, link.OutputID_);
		}

		//3.リンク生成
		{
			//ノードを生成していないときに処理
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

					//NOTE:ネストが深すぎませんか？
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
									TNodeManagerRef->MakeLink<ed::PinId>(startPinId, endPinId);
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
