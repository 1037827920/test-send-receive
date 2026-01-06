# test-send-receive
send/recv test

# 程序说明：

- receive-test：只接收ICMP包
- send-test：只发送ICMP包
- tcp-server：TCP服务器，为了走内核sk_clone()
- tcp-client：TCP客户端，为了走内核sk_clone()