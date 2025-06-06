/* SPDX-FileCopyrightText: 2006 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup cmpnodes
 */

#include "COM_node_operation.hh"

#include "node_composite_util.hh"

#include "UI_interface.hh"
#include "UI_resources.hh"

/* **************** VALUE ******************** */

namespace blender::nodes::node_composite_value_cc {

static void cmp_node_value_declare(NodeDeclarationBuilder &b)
{
  b.add_output<decl::Float>("Value").default_value(0.5f).custom_draw(
      [](CustomSocketDrawParams &params) {
        uiLayout &row = params.layout.row(true);
        row.prop(&params.socket_ptr, "default_value", UI_ITEM_NONE, "", ICON_NONE);
      });
  ;
}

using namespace blender::compositor;

class ValueOperation : public NodeOperation {
 public:
  using NodeOperation::NodeOperation;

  void execute() override
  {
    Result &result = get_result("Value");
    result.allocate_single_value();

    const bNodeSocket *socket = static_cast<bNodeSocket *>(bnode().outputs.first);
    float value = static_cast<bNodeSocketValueFloat *>(socket->default_value)->value;

    result.set_single_value(value);
  }
};

static NodeOperation *get_compositor_operation(Context &context, DNode node)
{
  return new ValueOperation(context, node);
}

}  // namespace blender::nodes::node_composite_value_cc

static void register_node_type_cmp_value()
{
  namespace file_ns = blender::nodes::node_composite_value_cc;

  static blender::bke::bNodeType ntype;

  cmp_node_type_base(&ntype, "CompositorNodeValue", CMP_NODE_VALUE);
  ntype.ui_name = "Value";
  ntype.ui_description = "Input numerical values to other nodes in the node graph";
  ntype.enum_name_legacy = "VALUE";
  ntype.nclass = NODE_CLASS_INPUT;
  ntype.declare = file_ns::cmp_node_value_declare;
  blender::bke::node_type_size_preset(ntype, blender::bke::eNodeSizePreset::Default);
  ntype.get_compositor_operation = file_ns::get_compositor_operation;
  ntype.gather_link_search_ops = nullptr;

  blender::bke::node_register_type(ntype);
}
NOD_REGISTER_NODE(register_node_type_cmp_value)
