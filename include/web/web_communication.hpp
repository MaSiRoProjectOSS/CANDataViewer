/**
 * @file web_communication.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-12-06
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_WEB_COMMUNICATION_HPP
#define MASIRO_PROJECT_TOY_BOX_WEB_COMMUNICATION_HPP

#include <WebServer.h>

namespace MaSiRoProject
{
namespace ToyBox
{
namespace WEB
{

class WebCommunication {
public:
    typedef std::function<void(bool, const char *, bool)> MessageFunction;

public:
    WebCommunication();
    ~WebCommunication();

public:
    bool setup();
    bool begin();
    bool set_callback_message(MessageFunction callback);

protected:
    WebServer *get_server();
    virtual bool setup_server(WebServer *server);
    virtual void handle_not_found();
    IPAddress get_ip();

private:
    bool initialized = false;
    MessageFunction callback_message;
    void happened_message(bool is_error, const char *message);
};
} // namespace WEB
} // namespace ToyBox
} // namespace MaSiRoProject
#endif
