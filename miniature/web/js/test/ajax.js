
if (!JS_AJAX) {
    var JS_AJAX = {
        countdown: 1,
        mode_id: 1,
        post: function (url) { return JS_AJAX.send("post", url); },
        get: function (url) { return JS_AJAX.send("get", url); },
        send: function (method, url) {
            console.log("JS_AJAX : " + url);
            return new Promise((resolve, reject) => {
                if (url == "/get/net_list") {
                    resolve({
                        subject: url,
                        result: 'OK', status: {
                            num: 200, messages: "",
                            data: [
                                {
                                    name: "test_a",
                                    power: 88
                                },
                                {
                                    name: "test_b",
                                    power: 86
                                },
                                {
                                    name: "test_c",
                                    power: 8
                                }
                            ]
                        }
                    });
                } else if (url == "/get/network") {
                    JS_AJAX.countdown--;
                    if (JS_AJAX.countdown <= 0) {
                        JS_AJAX.countdown = 3;
                        if (0 == JS_AJAX.mode_id) {
                            JS_AJAX.mode_id = 1;
                            resolve({
                                subject: url,
                                result: 'OK', status: {
                                    num: 200, messages: "",
                                    data: {
                                        ap_mode: 1,
                                        default: "ap-mode_default",
                                        name: "ap-mode"
                                    }
                                }
                            });
                        } else {
                            JS_AJAX.mode_id = 0;
                            resolve({
                                subject: url,
                                result: 'OK', status: {
                                    num: 200, messages: "",
                                    data: {
                                        ap_mode: 0,
                                        default: "sta-mode_default",
                                        name: "sta-mode"
                                    }
                                }
                            });
                        }
                    } else {
                        console.log("JS_AJAX : " + "[E] /get/network");
                        reject({
                            subject: url,
                            result: 'NG', status: { num: 500, messages: 'Internal Server Error' }
                        });
                    }
                } else {
                    if (false) {
                        reject({
                            subject: url,
                            result: 'NG', status: { num: 500, messages: 'Internal Server Error' }
                        });
                    } else {
                        resolve({
                            subject: url,
                            result: 'OK', status: {
                                num: 200, messages: ""
                            }
                        });
                    }
                }
            })
        }
    }
}
