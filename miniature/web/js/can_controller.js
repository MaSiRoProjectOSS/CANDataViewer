if (!JS_CCtrl) {
    var JS_CCtrl = {
        can_data: null,
        timerId: null,
        timerInterval: 1000,
        none: function (data) {
        },
        send_can: function (id, len, loop, data0, data1, data2, data3, data4, data5, data6, data7) {
            let num_loop = Number(loop) * 1000;
            JS_AJAX.get('/set/can_data' + '?id=' + id + '&len=' + len + (0 == num_loop ? '' : '&loop=' + num_loop)
                + '&d0=' + data0 + '&d1=' + data1 + '&d2=' + data2 + '&d3=' + data3
                + '&d4=' + data4 + '&d5=' + data5 + '&d6=' + data6 + '&d7=' + data7).then(
                    ok => JS_CCtrl.none(ok)
                    , error => console.error(error.status.messages)
                );
        },
        send: function () {
            JS_CCtrl.send_can(
                document.getElementById("b_send_id").value,
                document.getElementById("b_send_len").value,
                document.getElementById("send_time").innerText,
                document.getElementById("b_send_data_0").value, document.getElementById("b_send_data_1").value,
                document.getElementById("b_send_data_2").value, document.getElementById("b_send_data_3").value,
                document.getElementById("b_send_data_4").value, document.getElementById("b_send_data_5").value,
                document.getElementById("b_send_data_6").value, document.getElementById("b_send_data_7").value,
            );
        },
        one_more: function (table_type, id) {
            for (var i = 0; i < JS_CCtrl.can_data.resume.length; i++) {
                if (id == JS_CCtrl.can_data.resume[i].ID) {
                    JS_CCtrl.send_can(
                        JS_CCtrl.can_data.resume[i].ID,
                        JS_CCtrl.can_data.resume[i].LEN,
                        JS_CCtrl.can_data.resume[i].INTERVAL,
                        JS_CCtrl.can_data.resume[i].DATA[0], JS_CCtrl.can_data.resume[i].DATA[1],
                        JS_CCtrl.can_data.resume[i].DATA[2], JS_CCtrl.can_data.resume[i].DATA[3],
                        JS_CCtrl.can_data.resume[i].DATA[4], JS_CCtrl.can_data.resume[i].DATA[5],
                        JS_CCtrl.can_data.resume[i].DATA[6], JS_CCtrl.can_data.resume[i].DATA[7]
                    );
                    break;
                }
            }
        },
        clear: function (table_type) {
            JS_AJAX.get('/set/clear').then(
                ok => JS_CCtrl.none(ok)
                , error => console.error(error.status.messages)
            );
        },
        default: function (table_type) {
            JS_AJAX.get('/set/default').then(
                ok => JS_CCtrl.none(ok)
                , error => console.error(error.status.messages)
            );
        },
        delete_row: function (table_type, id) {
            JS_AJAX.get('/set/delete?id=' + id).then(
                ok => JS_CCtrl.none(ok)
                , error => console.error(error.status.messages)
            );
        },
        change_mode: function () {
            JS_AJAX.get('/set/change_mode').then(
                ok => JS_CCtrl.none(ok)
                , error => JS_CCtrl.none(error)
            );
        },
        make: function (data) {
            if (null != data) {
                if ("OK" == data.result) {
                    JS_CCtrl.can_data = data;
                    JS_Table.add_table(data.resume, 'send_one_shot', 'send_one_shot_size', 0);
                    JS_Table.add_table(data.send, 'send_loop', 'send_loop_size', 1);
                    JS_Table.add_table(data.received, 'received_table', 'received_size', 2);
                    let txt_mode = document.getElementById("can_mode");
                    if (txt_mode.innerHTML != data.status.mode) { txt_mode.innerHTML = data.status.mode; }
                    let txt_time = document.getElementById("current_time");
                    if (0 != data.time) {
                        txt_time.innerHTML = (Number(data.time) / 1000).toFixed(3);
                    } else { txt_time.innerHTML = "--"; }
                }
            }
        },
        interval: function () {
            JS_AJAX.get('/get/can_data').then(
                ok => JS_CCtrl.make(ok)
                , error => console.error(error.status.messages)
            );
        }
    }
}

window.onload = function () {
    JS_CCtrl.timerId =
        setInterval(JS_CCtrl.interval, JS_CCtrl.timerInterval);
};
window.onunload = function () {
    if (null != JS_CCtrl.timerId) {
        clearInterval(JS_CCtrl.timerId);
    }
};