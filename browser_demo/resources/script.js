function testCrossDomain() {
    var xhr = new XMLHttpRequest();
    xhr.open('get', 'http://music.163.com');
    xhr.send();
}

function bootstrap() {
    document.getElementById("send").onclick = function () {
        var msg = document.getElementById("msg").value;
        console.log("msg:", msg, ", dialog:", dialog);
        dialog && dialog.printMessage(msg);
    };
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


new QWebChannel(qt.webChannelTransport, function (channel) {
    window.channel = channel;
    window.dialog = channel.objects.dialog;
    bootstrap();
});