#pragma once

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <stdint.h>

namespace JCToolKit
{

#define SOCKET_DEFAULT_BUF_SIZE (256 * 1024)

    class SocketHandler
    {
    public:
        /**
     * 创建tcp客户端套接字并连接服务器
     * @param host 服务器ip或域名
     * @param port 服务器端口号
     * @param isAsync 是否异步连接
     * @param localIp 绑定的本地网卡ip
     * @param localPort 绑定的本地端口号
     * @return -1代表失败，其他为socket fd号
     */
        static int connect(const char *host, uint16_t port, bool isAsync = true, const char *localIp = "0.0.0.0", uint16_t localPort = 0);

        /**
     * 创建tcp监听套接字
     * @param port 监听的本地端口
     * @param localIp 绑定的本地网卡ip
     * @param backLog accept列队长度
     * @return -1代表失败，其他为socket fd号
     */
        static int listen(const uint16_t port, const char *localIp = "0.0.0.0", int backLog = 1024);

        /**
     * 创建udp套接字
     * @param port 监听的本地端口
     * @param localIp 绑定的本地网卡ip
     * @return -1代表失败，其他为socket fd号
     */
        static int bindUdpSock(const uint16_t port, const char *localIp = "0.0.0.0");

        /**
     * 绑定socket fd至某个网卡和端口
     * @param sock socket fd号
     * @param localIp 绑定的本地网卡ip
     * @param port 绑定的本地端口
     * @return 0代表成功，-1为失败
     */
        static int bindSock(int sock, const char *localIp, uint16_t port);

        /**
     * 开启TCP_NODELAY，降低TCP交互延时
     * @param sock socket fd号
     * @param on 是否开启
     * @return 0代表成功，-1为失败
     */
        static int setNoDelay(int sock, bool on = true);

        /**
     * 写socket不触发SIG_PIPE信号(貌似只有mac有效)
     * @param sock socket fd号
     * @return 0代表成功，-1为失败
     */
        static int setNoSigpipe(int sock);

        /**
     * 设置读写socket是否阻塞
     * @param sock socket fd号
     * @param noblock 是否阻塞
     * @return 0代表成功，-1为失败
     */
        static int setNoBlocked(int sock, bool noblock = true);

        /**
     * 设置socket接收缓存，默认貌似8K左右，一般有设置上限
     * 可以通过配置内核配置文件调整
     * @param sock socket fd号
     * @param size 接收缓存大小
     * @return 0代表成功，-1为失败
     */
        static int setRecvBuf(int sock, int size = SOCKET_DEFAULT_BUF_SIZE);

        /**
     * 设置socket接收缓存，默认貌似8K左右，一般有设置上限
     * 可以通过配置内核配置文件调整
     * @param sock socket fd号
     * @param size 接收缓存大小
     * @return 0代表成功，-1为失败
     */
        static int setSendBuf(int sock, int size = SOCKET_DEFAULT_BUF_SIZE);

        /**
     * 设置后续可绑定复用端口(处于TIME_WAITE状态)
     * @param sock socket fd号
     * @param on 是否开启该特性
     * @return 0代表成功，-1为失败
     */
        static int setReuseable(int sock, bool on = true);

        /**
     * 运行发送或接收udp广播信息
     * @param sock socket fd号
     * @param on 是否开启该特性
     * @return 0代表成功，-1为失败
     */
        static int setBroadcast(int sock, bool on = true);

        /**
     * 是否开启TCP KeepAlive特性
     * @param sock socket fd号
     * @param on 是否开启该特性
     * @return 0代表成功，-1为失败
     */
        static int setKeepAlive(int sock, bool on = true);

        /**
     * 是否开启FD_CLOEXEC特性(多进程相关)
     * @param fd fd号，不一定是socket
     * @param on 是否开启该特性
     * @return 0代表成功，-1为失败
     */
        static int setCloExec(int fd, bool on = true);

        /**
     * 开启SO_LINGER特性
     * @param sock socket fd号
     * @param second 内核等待关闭socket超时时间，单位秒
     * @return 0代表成功，-1为失败
     */
        static int setCloseWait(int sock, int second = 0);

        /**
     * dns解析
     * @param host 域名或ip
     * @param port 端口号
     * @param addr sockaddr结构体
     * @return 是否成功
     */
        static bool getDomainIP(const char *host, uint16_t port, struct sockaddr &addr);

        /**
     * 设置组播ttl
     * @param sock socket fd号
     * @param ttl ttl值
     * @return 0代表成功，-1为失败
     */
        static int setMultiTTL(int sock, uint8_t ttl = 64);

        /**
     * 设置组播发送网卡
     * @param sock socket fd号
     * @param strLocalIp 本机网卡ip
     * @return 0代表成功，-1为失败
     */
        static int setMultiIF(int sock, const char *strLocalIp);

        /**
     * 设置是否接收本机发出的组播包
     * @param sock socket fd号
     * @param bAccept 是否接收
     * @return 0代表成功，-1为失败
     */
        static int setMultiLOOP(int sock, bool bAccept = false);

        /**
     * 加入组播
     * @param sock socket fd号
     * @param strAddr 组播地址
     * @param strLocalIp 本机网卡ip
     * @return 0代表成功，-1为失败
     */
        static int joinMultiAddr(int sock, const char *strAddr, const char *strLocalIp = "0.0.0.0");

        /**
     * 退出组播
     * @param sock socket fd号
     * @param strAddr 组播地址
     * @param strLocalIp 本机网卡ip
     * @return 0代表成功，-1为失败
     */
        static int leaveMultiAddr(int sock, const char *strAddr, const char *strLocalIp = "0.0.0.0");

        /**
     * 加入组播并只接受该源端的组播数据
     * @param sock socket fd号
     * @param strAddr 组播地址
     * @param strSrcIp 数据源端地址
     * @param strLocalIp  本机网卡ip
     * @return 0代表成功，-1为失败
     */
        static int joinMultiAddrFilter(int sock, const char *strAddr, const char *strSrcIp, const char *strLocalIp = "0.0.0.0");

        /**
     * 退出组播
     * @param sock socket fd号
     * @param strAddr 组播地址
     * @param strSrcIp 数据源端地址
     * @param strLocalIp  本机网卡ip
     * @return 0代表成功，-1为失败
     */
        static int leaveMultiAddrFilter(int sock, const char *strAddr, const char *strSrcIp, const char *strLocalIp = "0.0.0.0");

        /**
     * 获取该socket当前发生的错误
     * @param sock socket fd号
     * @return 错误代码
     */
        static int getSockError(int sock);

        /**
     * 获取网卡列表
     * @return vector<map<ip:name> >
     */
        static std::vector<std::map<std::string, std::string>> getInterfaceList();

        /**
     * 获取本机默认网卡ip
     */
        static std::string get_local_ip();

        /**
     * 获取该socket绑定的本地ip
     * @param sock socket fd号
     */
        static std::string get_local_ip(int sock);

        /**
     * 获取该socket绑定的本地端口
     * @param sock socket fd号
     */
        static uint16_t get_local_port(int sock);

        /**
     * 获取该socket绑定的远端ip
     * @param sock socket fd号
     */
        static std::string get_peer_ip(int sock);

        /**
     * 获取该socket绑定的远端端口
     * @param sock socket fd号
     */
        static uint16_t get_peer_port(int sock);

        /**
     * 线程安全的in_addr转ip字符串
     */
        static std::string inet_ntoa(struct in_addr &addr);

        /**
     * 获取网卡ip
     * @param ifrName 网卡名
     */
        static std::string get_ifr_ip(const char *ifrName);

        /**
     * 获取网卡名
     * @param localIp 网卡ip
     */
        static std::string get_ifr_name(const char *localIp);

        /**
     * 根据网卡名获取子网掩码
     * @param ifrName 网卡名
     */
        static std::string get_ifr_mask(const char *ifrName);

        /**
     * 根据网卡名获取广播地址
     * @param ifrName 网卡名
     */
        static std::string get_ifr_brdaddr(const char *ifrName);

        /**
     * 判断两个ip是否为同一网段
     * @param myIp 我的ip
     * @param dstIp 对方ip
     */
        static bool in_same_lan(const char *myIp, const char *dstIp);
    };

}