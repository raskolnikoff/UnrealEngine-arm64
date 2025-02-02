﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MassProcessor.h"
#include "MassEntitySubsystem.h"
#include "MassSignalTypes.h"
#include "Containers/StaticArray.h"
#include "MassSignalProcessorBase.generated.h"

class UMassSignalSubsystem;

/** 
 * Processor for executing signals on each targeted entities
 * The derived classes only need to implement the method SignalEntities to actually received the raised signals for the entities they subscribed to 
 */
UCLASS(abstract)
class MASSSIGNALS_API UMassSignalProcessorBase : public UMassProcessor
{
	GENERATED_BODY()

public:
	UMassSignalProcessorBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/**
	 * Initialize the processor
	 * @param Owner of the Processor
	 */
	virtual void Initialize(UObject& Owner) override;

	virtual void BeginDestroy() override;

	/** Configure the owned FMassEntityQuery instances to express processor queries requirements */
	virtual void ConfigureQueries() override {}

	/**
	 * Actual method that derived class needs to implement to act on a signal that is raised for that frame
	 * @param EntitySubsystem is the system to execute the lambdas on each entity chunk
	 * @param Context is the execution context to be passed when executing the lambdas
	 * @param EntitySignals Look up to retrieve for each entities their raised signal via GetSignalsForEntity
	 */
	 virtual void SignalEntities(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context, FMassSignalNameLookup& EntitySignals) PURE_VIRTUAL(UMassSignalProcessorBase::SignalEntities, );

	/**
	 * Callback that is being called when new signal is raised
	 * @param SignalName is the name of the signal being raised
	 * @param Entities are the targeted entities for this signal
	 */
	 virtual void OnSignalReceived(FName SignalName, TConstArrayView<FMassEntityHandle> Entities);

	/**
	 * Execution method for this processor
	 * @param EntitySubsystem is the system to execute the lambdas on each entity chunk
	 * @param Context is the execution context to be passed when executing the lambdas
	 */
	virtual void Execute(UMassEntitySubsystem& EntitySubsystem, FMassExecutionContext& Context) override;

	/**
	 * To receive notification about a particular signal, you need to subscribe to it.
	 * @param SignalName is the name of the signal to receive notification about
	 */
	void SubscribeToSignal(const FName SignalName);

	
	UPROPERTY(Transient)
	UMassSignalSubsystem* SignalSubsystem = nullptr;

	FMassEntityQuery EntityQuery;

private:

	/** Stores a range of indices in the SignaledEntities TArray of Entities and the associated signal name */
	struct FEntitySignalRange
	{
		FName SignalName;
		int32 Begin = 0;
		int32 End = 0;
		bool bProcessed = false;
	};

	struct FFrameReceivedSignals
	{
		/** Received signals are double buffered as we can receive new one while processing them */
		TArray<FEntitySignalRange> ReceivedSignalRanges;

		/** the list of all signaled entities, can contain duplicates */
		TArray<FMassEntityHandle> SignaledEntities;
	};

	/** Double buffer frame received signal as we can receive new signals as we are processing them */
	TStaticArray<FFrameReceivedSignals, 2> FrameReceivedSignals;
	
	/** Current frame buffer index of FrameReceivedSignals */
	int32 CurrentFrameBufferIndex = 0;

	/** Lookup used to store and retrieve signals per entity, only used during processing */
	FMassSignalNameLookup SignalNameLookup;

	/** List of all the registered signal names*/
	TArray<FName> RegisteredSignals;
};

