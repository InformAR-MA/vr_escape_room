// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"

class Message {
protected:
	FString data;
	FString recipient;

public:
	Message(FString recipient, FString message);
	FString ToString();
};

class MessageEventParams {
public:
	FString message;
	FString sender;

	MessageEventParams(FString sender, FString message);
};

class ErrorEventParams {
public:
	FString reason;
	int code;

	ErrorEventParams(int code, FString reason);
};

DECLARE_EVENT_TwoParams(MClient, FMessageEvent, FString, FString);
DECLARE_EVENT_TwoParams(MClient, FErrorEvent, int, FString);

class MYPROJECT2_API MClient
{
private:
	bool connected;
	TSharedPtr<IWebSocket> socket;

	FMessageEvent MessageEvent;
	FErrorEvent ErrorEvent;

public:
	FMessageEvent& OnMessage() { return MessageEvent; }
	FErrorEvent& OnError() { return ErrorEvent; }

	MClient(FString url, FString protocol);
	~MClient();

	void Send(Message* message);
};