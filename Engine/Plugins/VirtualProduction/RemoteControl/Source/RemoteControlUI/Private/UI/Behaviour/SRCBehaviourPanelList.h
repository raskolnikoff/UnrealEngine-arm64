﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UI/BaseLogicUI/SRCLogicPanelListBase.h"

class URCBehaviour;
class FRCControllerModel;
class FRCBehaviourModel;
class ITableRow;
class ITableBase;
class SRCBehaviourPanel;
class STableViewBase;
class URemoteControlPreset;
template <typename ItemType> class SListView;

/*
* ~ SRCBehaviourPanelList ~
*
* UI Widget for Behaviours List
* Used as part of the RC Logic Behaviour Panel.
*/
class REMOTECONTROLUI_API SRCBehaviourPanelList : public SRCLogicPanelListBase
{
public:
	SLATE_BEGIN_ARGS(SRCBehaviourPanelList)
	{
	}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, const TSharedRef<SRCBehaviourPanel> InBehaviourPanel, TSharedPtr<FRCControllerModel> InControllerItem);

private:
	/** OnGenerateRow delegate for the Behaviours List View */
	TSharedRef<ITableRow> OnGenerateWidgetForList( TSharedPtr<FRCBehaviourModel> InItem, const TSharedRef<STableViewBase>& OwnerTable );
	
	/** OnSelectionChanged delegate for Behaviours List View */
	void OnTreeSelectionChanged(TSharedPtr<FRCBehaviourModel> InItem , ESelectInfo::Type);

	/** Responds to the selection of a newly created Behaviour. Resets UI state */
	void OnBehaviourAdded(URCBehaviour* InBehaviour);

	/** Responds to the removal of all Behaviours. Rests UI state */
	void OnEmptyBehaviours();

	/** The parent Behaviour Panel widget */
	TWeakPtr<SRCBehaviourPanel> BehaviourPanelWeakPtr;
	
	/** The Controller (UI model) associated with us */
	TWeakPtr<FRCControllerModel> ControllerItemWeakPtr;
	
	/** The currently selected Behaviour item (UI model) */
	TWeakPtr<FRCBehaviourModel> SelectedBehaviourItemWeakPtr;

	/** List of Behaviours (UI models) active in this widget */
	TArray<TSharedPtr<FRCBehaviourModel>> BehaviourItems;
	
	/** List View widget for representing our Behaivours List*/
	TSharedPtr<SListView<TSharedPtr<FRCBehaviourModel>>> ListView;

	/** Refreshes the list from the latest state of the model*/
	virtual void Reset() override;

	/** Handles broadcasting of a successful remove item operation.*/
	virtual void BroadcastOnItemRemoved() override;

	/** Fetches the Remote Control preset associated with the parent panel */
	virtual URemoteControlPreset* GetPreset() override;

	/** Removes the given Behaviour UI model item from the list of UI models*/
	virtual int32 RemoveModel(const TSharedPtr<FRCLogicModeBase> InModel) override;

public:
	/** Whether the Behaviours List View currently has focus.*/
	bool IsListFocused() const;

	/** Deletes currently selected items from the list view*/
	void DeleteSelectedPanelItem();
};

