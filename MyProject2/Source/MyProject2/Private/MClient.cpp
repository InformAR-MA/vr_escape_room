// Fill out your copyright notice in the Description page of Project Settings.


#include "MClient.h"
#include "Modules/ModuleManager.h"

Message::Message(FString recipient, FString data) {
    this->recipient = recipient;
    this->data = data;
}

FString Message::ToString() {
    TArray<FStringFormatArg> args;
    args.Add(this->data);
    args.Add(this->recipient);

    return FString::Format(TEXT("{\"type\": \"message\", \"data\": \"{0}\", \"recipient\": \"{1}\"}"), args);
}

MessageEventParams::MessageEventParams(FString sender, FString message) {
    this->sender = sender;
    this->message = message;
}

ErrorEventParams::ErrorEventParams(int code, FString reason) {
    this->code = code;
    this->reason = reason;
}

MClient::MClient(FString url, FString protocol) {
    if (!FModuleManager::Get().IsModuleLoaded("WebSockets")) FModuleManager::Get().LoadModule("WebSockets");

    connected = false;

    UE_LOG(LogTemp, Warning, TEXT("%s"), *url);

    socket = FWebSocketsModule::Get().CreateWebSocket(url, protocol);

    socket->OnConnected().AddLambda([this]() -> void {
        socket->Send("{\"type\": \"identify\", \"id\": \"Unreal\"}");
        connected = true;
        });

    socket->OnConnectionError().AddLambda([this](const FString& Error) -> void {
        connected = false;
        });

    socket->OnClosed().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClean) -> void {
        connected = false;
        });

    socket->OnMessage().AddLambda([this](const FString& Message) -> void {
        TSharedPtr<FJsonObject> jsonObject;
        TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Message);

        if (FJsonSerializer::Deserialize(reader, jsonObject)) {
            FString type = jsonObject->GetStringField("type");

            if (type.Equals("message")) {
                FString data = jsonObject->GetStringField("data");
                FString sender = jsonObject->GetStringField("sender");

                MessageEvent.Broadcast(sender, data);
                return;
            }

            if (type.Equals("error")) {
                int code = jsonObject->GetIntegerField("code");
                FString reason = jsonObject->GetStringField("reason");

                ErrorEvent.Broadcast(code, reason);
                return;
            }

            if (type.Equals("identify")) {
                FString id = jsonObject->GetStringField("id");
                return;
            }
        }
        });

    socket->OnRawMessage().AddLambda([](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) -> void {});

    socket->OnMessageSent().AddLambda([](const FString& MessageString) -> void {});

    socket->Connect();

}

void MClient::Send(Message* message) {
    socket->Send(message->ToString());
}

MClient::~MClient()
{
    socket->Close();
    delete& socket;
}