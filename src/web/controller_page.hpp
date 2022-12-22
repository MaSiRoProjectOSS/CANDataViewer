/**
 * @file controller_page.hpp
 * @author Akari (masiro.to.akari@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-26
 *
 * @copyright Copyright (c) 2022 / MaSiRo Project.
 *
 */
#ifndef MASIRO_PROJECT_TOY_BOX_CONTROLLER_PAGE
#define MASIRO_PROJECT_TOY_BOX_CONTROLLER_PAGE

#include "web_communication.hpp"

namespace MaSiRoProject
{
namespace WEB
{

class ControllerPage : public WEB::WebCommunication {
public:
    typedef std::function<std::vector<CanData>(void)> RequestCanDataFunction;
    typedef std::function<bool(CanData)> SetCanDataFunction;
    typedef std::function<CanDeviceInfo(void)> GetCanInfoFunction;
    typedef std::function<bool(CAN_CTRL_STATE)> SetCanModeFunction;
    typedef std::function<bool(int)> SetAnyFunction;

public:
    ControllerPage();
    ~ControllerPage();

public:
    bool setup_callback(RequestCanDataFunction data_request_send,
                        RequestCanDataFunction data_request_received,
                        RequestCanDataFunction data_request_resume,
                        GetCanInfoFunction device_info,
                        SetCanModeFunction can_mode,
                        SetCanDataFunction on_shot,
                        SetCanDataFunction loop,
                        SetAnyFunction data_clear,
                        SetAnyFunction data_default,
                        SetAnyFunction data_delete);

protected:
    bool setup_server(WebServer *server) override;

private:
    RequestCanDataFunction callback_data_request_send;
    RequestCanDataFunction callback_data_request_received;
    RequestCanDataFunction callback_data_request_resume;
    SetCanDataFunction callback_on_shot;
    SetCanDataFunction callback_loop;
    GetCanInfoFunction callback_device_info;
    SetCanModeFunction callback_can_mode;
    SetAnyFunction callback_data_clear;
    SetAnyFunction callback_data_default;
    SetAnyFunction callback_data_delete;
    void set_mode_on();
    void set_mode_off();
    void set_can_data();
    void get_can_data();
    void set_change_mode();
    void set_clear();
    void set_default();
    void set_delete();

private:
    String handle_page(const char *body);
    std::string page();
    void handle_root();
    void handle_css();
    void handle_js_ajax();
    void handle_js_can_controller();
    void handle_js_table_view();

    std::string get_can_data_text(RequestCanDataFunction callback);
};
} // namespace WEB
} // namespace MaSiRoProject
#endif
