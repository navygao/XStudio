#pragma once
#ifndef __XGOODS_H__
#define __XGOODS_H__

struct stBaseGoodsInfo;

namespace xgc
{
	class CORE_API XGoods : public XObject
	{
		DECLARE_XCLASS();
		static xAttrIndex Index;	///< 物品配置索引
		static xAttrIndex Sequence;	///< 物品全局索引
		static xAttrIndex Alias;	///< 物品别名

	protected:
		XGoods();

		~XGoods();

	public:
		///
		/// 比较物品,排序时使用，在GS层实现 
		/// [8/4/2014] create by wuhailin.jerry
		///
		virtual xgc_bool Compare( xObject hObject ) = 0;

		///
		/// 物品最大堆叠个数 
		/// [9/2/2014] create by wuhailin.jerry
		///
		virtual xgc_uint32 GetOverlapNum() = 0;
	};
}

#endif // __XGOODS_H__