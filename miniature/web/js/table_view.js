
if (!JS_Table) {
    var JS_Table = {
        del_head: function (elm) {
            let tbody = document.getElementById(elm);
            while (2 < tbody.rows.length) {
                tbody.deleteRow(tbody.rows.length - 1);
            }
        },
        change: function () {
            if ((0 == document.getElementById("b_send_time_s").value) && (0 == document.getElementById("b_send_time_ms").value)) {
                document.getElementById("send_type").innerHTML = "One shot";
                document.getElementById("send_time").innerHTML = "0.000";
            } else {
                document.getElementById("send_type").innerHTML = "Loop";
                if (1000 == document.getElementById("b_send_time_ms").value) {
                    document.getElementById("send_time").innerHTML = (Number(document.getElementById("b_send_time_s").value) + 1) + ".000";
                } else {
                    document.getElementById("send_time").innerHTML = document.getElementById("b_send_time_s").value + "." +
                        document.getElementById("b_send_time_ms").value.toString().padStart(3, '0');
                }
            }
            document.getElementById("send_id").innerHTML = "0x" + Number(document.getElementById("b_send_id").value).toString(16);
            document.getElementById("send_len").innerHTML = document.getElementById("b_send_len").value;
            document.getElementById("send_data_0").innerHTML = "0x" + Number(document.getElementById("b_send_data_0").value).toString(16);
            document.getElementById("send_data_1").innerHTML = "0x" + Number(document.getElementById("b_send_data_1").value).toString(16);
            document.getElementById("send_data_2").innerHTML = "0x" + Number(document.getElementById("b_send_data_2").value).toString(16);
            document.getElementById("send_data_3").innerHTML = "0x" + Number(document.getElementById("b_send_data_3").value).toString(16);
            document.getElementById("send_data_4").innerHTML = "0x" + Number(document.getElementById("b_send_data_4").value).toString(16);
            document.getElementById("send_data_5").innerHTML = "0x" + Number(document.getElementById("b_send_data_5").value).toString(16);
            document.getElementById("send_data_6").innerHTML = "0x" + Number(document.getElementById("b_send_data_6").value).toString(16);
            document.getElementById("send_data_7").innerHTML = "0x" + Number(document.getElementById("b_send_data_7").value).toString(16);
        },
        copy_row: function (table_type, id) {
            for (var i = 0; i < JS_CCtrl.can_data.resume.length; i++) {
                if (id == JS_CCtrl.can_data.resume[i].ID) {
                    document.getElementById("b_send_id").value = JS_CCtrl.can_data.resume[i].ID;
                    document.getElementById("b_send_len").value = JS_CCtrl.can_data.resume[i].LEN;
                    if (0 == JS_CCtrl.can_data.resume[i].INTERVAL) {
                        document.getElementById("b_send_time_s").value = 0;
                        document.getElementById("b_send_time_ms").value = 0;
                    } else {
                        document.getElementById("b_send_time_ms").value = (JS_CCtrl.can_data.resume[i].INTERVAL % 1000);
                        document.getElementById("b_send_time_s").value =
                            (JS_CCtrl.can_data.resume[i].INTERVAL - document.getElementById("b_send_time_ms").value) / 1000;
                    }
                    document.getElementById("b_send_data_0").value = JS_CCtrl.can_data.resume[i].DATA[0];
                    document.getElementById("b_send_data_1").value = JS_CCtrl.can_data.resume[i].DATA[1];
                    document.getElementById("b_send_data_2").value = JS_CCtrl.can_data.resume[i].DATA[2];
                    document.getElementById("b_send_data_3").value = JS_CCtrl.can_data.resume[i].DATA[3];
                    document.getElementById("b_send_data_4").value = JS_CCtrl.can_data.resume[i].DATA[4];
                    document.getElementById("b_send_data_5").value = JS_CCtrl.can_data.resume[i].DATA[5];
                    document.getElementById("b_send_data_6").value = JS_CCtrl.can_data.resume[i].DATA[6];
                    document.getElementById("b_send_data_7").value = JS_CCtrl.can_data.resume[i].DATA[7];

                    JS_Table.change();
                    break;
                }
            }
        },
        add_table: function (data, elm_ta, elm_size, table_type) {
            if (null != data) {
                let len = data.length;
                let l_size = 0;
                let t_size = document.getElementById(elm_size);
                JS_Table.del_head(elm_ta);
                let tbody = document.getElementById(elm_ta);
                for (i = 0; i < len; i++) {
                    if (0 == table_type) {
                        if (0 != data[i].INTERVAL) {
                            continue;
                        }
                    }
                    l_size++;
                    let tr = document.createElement("tr");
                    if ((0 == table_type) || (1 == table_type)) {
                        let td_del = document.createElement("th");
                        let elm_input_del = document.createElement("input");
                        elm_input_del.type = "button";
                        elm_input_del.value = "delete";
                        elm_input_del.setAttribute('onclick', "JS_CCtrl.delete_row(" + table_type + "," + data[i].ID + ");");
                        td_del.appendChild(elm_input_del);
                        tr.appendChild(td_del);

                        let td_copy = document.createElement("th");
                        let elm_input_copy = document.createElement("input");
                        elm_input_copy.type = "button";
                        elm_input_copy.value = "copy";
                        elm_input_copy.setAttribute('onclick', "JS_Table.copy_row(" + table_type + "," + data[i].ID + ");");
                        td_copy.appendChild(elm_input_copy);
                        tr.appendChild(td_copy);
                    }
                    let td_time = document.createElement("th");
                    td_time.innerHTML = (data[i].TIME / 1000).toFixed(3);
                    tr.appendChild(td_time);
                    if (1 == table_type) {
                        let td_loop = document.createElement("td");
                        td_loop.innerHTML = (data[i].INTERVAL / 1000).toFixed(3);
                        tr.appendChild(td_loop);
                    }
                    let td_id = document.createElement("td");
                    td_id.innerHTML = "0x" + data[i].ID.toString(16);
                    tr.appendChild(td_id);
                    let td_len = document.createElement("td");
                    let td_size = data[i].LEN;
                    td_len.innerHTML = td_size;
                    tr.appendChild(td_len);
                    for (let j = 0; j < td_size; j++) {
                        let td_data = document.createElement("td");
                        td_data.innerHTML = "0x" + data[i].DATA[j].toString(16);
                        tr.appendChild(td_data);
                    }
                    if (8 > td_size) {
                        let td_data = document.createElement("td");
                        td_data.colSpan = 8 - td_size;
                        tr.appendChild(td_data);
                    }
                    if (0 == table_type) {
                        let td_one_more = document.createElement("th");
                        let elm_input_one_more = document.createElement("input");
                        elm_input_one_more.type = "button";
                        elm_input_one_more.value = "one more";
                        elm_input_one_more.setAttribute('onclick', "JS_CCtrl.one_more(" + table_type + "," + data[i].ID + ");");
                        td_one_more.appendChild(elm_input_one_more);
                        tr.appendChild(td_one_more);
                    }
                    tbody.appendChild(tr);
                }
                t_size.innerHTML = l_size;
            }
        }
    }
}