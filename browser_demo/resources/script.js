
"use strict";

console.log("LOADING script.js");

function testCrossDomain() {
    var xhr = new XMLHttpRequest();
    xhr.open('get', 'http://music.163.com');
    xhr.send();
}

function channelCount() {
    qWebChannel.objects.channel.count(function(id) {
        console.log("id: ", id);
    });
}

function execSql() {
    var id = "query-1";
    var statement = "statement-1";
    qWebChannel.objects.channel.execSql(id, statement);
}

function printMessage(msg) {
    qWebChannel.objects.channel.printMessage(msg);
}

function onConfirmButtonClicked() {
    var ret = confirm("Confirm?");
    console.log("confirm result: ", ret);
}

function onPromptButtonClicked() {
    var ret = prompt("Please input your name:");
    console.log("prompt returns: ", ret);
}

function closeWindow() {
    window.close();
}

function popupWindow() {
    window.open("http://www.z.cn");
}

function onNotificationButtonClicked() {
    var msgTitle = "Message title";
    var msgOption = {
        body: "Message body!",
        icon: "qrc://images/user-home-symbolic.svg",
        tag: "tag",
    }
    // Let's check if the browser supports notifications
    if (!("Notification" in window)) {
        alert("This browser does not support system notifications");
    } else if (Notification.permission === "granted") {
        // Let's check whether notification permissions have already been granted
        // If it's okay let's create a notification
        var notification = new Notification(msgTitle, msgOption);
    } else if (Notification.permission !== 'denied') {
        // Otherwise, we need to ask the user for permission
        Notification.requestPermission(function (permission) {
            // If the user accepts, let's create a notification
            if (permission === "granted") {
                var notification = new Notification(msgTitle, msgOption);
            }
        });
    }

    // Finally, if the user has denied notifications and you
    // want to be respectful there is no need to bother them any more.
}

function redirectPage() {
    window.location.href = "qrc://resources/redirect.html";
}

function selectImageFiles() {
    const file = document.querySelector("input.avatar-file");
    if (file) {
        const event = new MouseEvent("click", {
            bubbles: true,
            cancelable: true,
            view: window
        });
        file.dispatchEvent(event);
    }
}

function updateButtonClickCount() {
    const key = "button-click";
    const val = getButtonClickCount();
    window.localStorage.setItem(key, val + 1);
    updateButtonClickLabel();
}

function updateButtonClickLabel() {
    const span = document.querySelector(".local-storage-label");
    span.innerHTML = getButtonClickCount();
}

function getButtonClickCount() {
    const key = "button-click";
    var val = window.localStorage.getItem(key);
    if (val !== null) {
        return parseInt(val);
    } else {
        return 0;
    }
}

function bootstrap() {
    qWebChannel.objects.channel.onExecSqlDone.connect(function(id, state, result) {
        console.log("id: ", id, ", state: ", state, ", result:", result);
    });

    document.getElementById("send").onclick = function () {
        var msg = document.getElementById("msg").value;
        printMessage(msg);
    };

    updateButtonClickLabel();
}

new QWebChannel(qt.webChannelTransport, function (channel) {
    window.qWebChannel = channel;
    bootstrap();
});