/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: faceDataMessage.h
* Purpose:
* Creation Date: 2017.12.11
* Created By: sh
* Compile :
*********************************************************/

#ifndef FACEDATA_MESSAGE_STRUCT_H_
#define FACEDATA_MESSAGE_STRUCT_H_

#include <vector>
#include <string>
#include <chrono>
#include "common/json/json.h"
#include "common.h"
namespace dev
{

template <typename Cls>
struct FaceDataMessage
{
    FaceDataMessage()
    {
    }
    FaceDataMessage(Json::Value &_request, const int _msgType) : request(_request), msgType(_msgType)
    {
    }

    FaceDataMessage(const FaceDataMessage &msg)
    {
        *this = msg;
    }

    FaceDataMessage(FaceDataMessage &&msg) noexcept : request(std::move(msg.request)), msgType(msg.msgType)
    {
        msg.request.clear();
        msg.msgType = 0;
    }

    FaceDataMessage &operator=(const FaceDataMessage &msg)
    {
        if (&msg == this)
            return *this;
        request = msg.request;
        msgType = msg.msgType;
        return *this;
    }

    FaceDataMessage &operator=(FaceDataMessage &&msg) noexcept
    {
        if (&msg == this)
            return *this;
        request = std::move(msg.request);
        msgType = msg.msgType;
        msg.request.clear();
        msg.msgType = 0;
        return *this;
    }

    Json::Value request;
    int msgType;
};
}
#endif
