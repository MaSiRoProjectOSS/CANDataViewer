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

#include <can_data_viewer_info.hpp>
#include <cushy_web_server.hpp>

namespace MaSiRoProject
{
namespace WEB
{

class ControllerPage : public CushyWebServer {
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
    bool setup_server(AsyncWebServer *server) override;
    void handle_favicon_ico(AsyncWebServerRequest *request) override;

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

private:
    void set_mode_on(AsyncWebServerRequest *request);
    void set_mode_off(AsyncWebServerRequest *request);
    void set_can_data(AsyncWebServerRequest *request);
    void get_can_data(AsyncWebServerRequest *request);
    void set_change_mode(AsyncWebServerRequest *request);
    void set_clear(AsyncWebServerRequest *request);
    void set_default(AsyncWebServerRequest *request);
    void set_delete(AsyncWebServerRequest *request);
    void handle_root(AsyncWebServerRequest *request);
    void handle_css(AsyncWebServerRequest *request);
    void handle_js_can_controller(AsyncWebServerRequest *request);
    void handle_js_table_view(AsyncWebServerRequest *request);

private:
    std::string page_html(const std::string body);
    std::string page_body();

    std::string get_can_data_text(RequestCanDataFunction callback);
};
} // namespace WEB
} // namespace MaSiRoProject
#endif
