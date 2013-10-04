//! \file SvgImport.h
//! \brief 定义SVG导入功能入口类 SvgImportGate

#ifndef SVGIMPORT_GATE_H_
#define SVGIMPORT_GATE_H_

#include "mgviewhandle.h"

//! SVG导入的功能入口类
/*! \ingroup CORE_VIEW
    参数 mgView 为 GiCoreView 的 viewAdapter() 返回值，可转换为 MgView 指针。
 */
struct SvgImportGate {
    //! 登记绘图命令和图形类型
    static int registerCmds(MGVIEW_HANDLE mgView);
};

#endif // SVGIMPORT_GATE_H_
