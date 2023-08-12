# Copyright (c) 2023, Henrik Söderlund

"""
Builds MicroXRCE_DDS
"""

load(
    "@com_github_mvukov_rules_ros2//ros2:cc_defs.bzl",
    "ros2_cpp_library",
)
load(
    "@com_github_mvukov_rules_ros2//third_party:expand_template.bzl",
    "expand_template",
)

TRANSPORT_SRCS = [
    "src/cpp/transport/udp/UDPv4AgentLinux.cpp",
    "src/cpp/transport/udp/UDPv6AgentLinux.cpp",
    "src/cpp/transport/tcp/TCPv4AgentLinux.cpp",
    "src/cpp/transport/tcp/TCPv6AgentLinux.cpp",
    "src/cpp/transport/serial/SerialAgentLinux.cpp",
    "src/cpp/transport/serial/TermiosAgentLinux.cpp",
    "src/cpp/transport/serial/MultiSerialAgentLinux.cpp",
    "src/cpp/transport/serial/MultiTermiosAgentLinux.cpp",
    "src/cpp/transport/serial/PseudoTerminalAgentLinux.cpp",
]

SRCS = [
    "src/cpp/Agent.cpp",
    "src/cpp/AgentInstance.cpp",
    "src/cpp/Root.cpp",
    "src/cpp/processor/Processor.cpp",
    "src/cpp/client/ProxyClient.cpp",
    "src/cpp/participant/Participant.cpp",
    "src/cpp/topic/Topic.cpp",
    "src/cpp/publisher/Publisher.cpp",
    "src/cpp/subscriber/Subscriber.cpp",
    "src/cpp/datawriter/DataWriter.cpp",
    "src/cpp/datareader/DataReader.cpp",
    "src/cpp/requester/Requester.cpp",
    "src/cpp/replier/Replier.cpp",
    "src/cpp/object/XRCEObject.cpp",
    "src/cpp/types/XRCETypes.cpp",
    "src/cpp/types/MessageHeader.cpp",
    "src/cpp/types/SubMessageHeader.cpp",
    "src/cpp/message/InputMessage.cpp",
    "src/cpp/message/OutputMessage.cpp",
    "src/cpp/utils/ArgumentParser.cpp",
    "src/cpp/transport/Server.cpp",
    "src/cpp/transport/stream_framing/StreamFramingProtocol.cpp",
    "src/cpp/transport/custom/CustomAgent.cpp",
    # UAGENT_FAST_PROFILE
    "src/cpp/types/TopicPubSubType.cpp",
    "src/cpp/xmlobjects/xmlobjects.cpp",
    "src/cpp/middleware/fast/FastEntities.cpp",
    "src/cpp/middleware/fast/FastMiddleware.cpp",
    "src/cpp/middleware/fastdds/FastDDSEntities.cpp",
    "src/cpp/middleware/fastdds/FastDDSMiddleware.cpp",
] + TRANSPORT_SRCS

expand_template(
    name = "config_hpp",
    out = "include/uxr/agent/config.hpp",
    substitutions = {
        "#cmakedefine UAGENT_FAST_PROFILE": "#define UAGENT_FAST_PROFILE",
        "#cmakedefine UAGENT_CED_PROFILE": "// #define UAGENT_CED_PROFILE",
        "#cmakedefine UAGENT_DISCOVERY_PROFILE": "// #define UAGENT_DISCOVERY_PROFILE",
        "#cmakedefine UAGENT_P2P_PROFILE": "// #define UAGENT_P2P_PROFILE",
        "#cmakedefine UAGENT_SOCKETCAN_PROFILE": "// #define UAGENT_SOCKETCAN_PROFILE",
        "#cmakedefine UAGENT_LOGGER_PROFILE": "#define UAGENT_LOGGER_PROFILE",
        "#cmakedefine UAGENT_TWEAK_XRCE_WRITE_LIMIT": "#define UAGENT_TWEAK_XRCE_WRITE_LIMIT",
        "@UAGENT_CONFIG_RELIABLE_STREAM_DEPTH@": "16",  # Reliable streams depth.
        "@UAGENT_CONFIG_BEST_EFFORT_STREAM_DEPTH@": "16",  # Best-effort streams depth.
        "@UAGENT_CONFIG_HEARTBEAT_PERIOD@": "200",  # Heartbeat period in milliseconds.
        "@UAGENT_CONFIG_TCP_MAX_CONNECTIONS@": "100",  # Maximum TCP connection allowed.
        "@UAGENT_CONFIG_TCP_MAX_BACKLOG_CONNECTIONS@": "100",  # Maximum TCP backlog connection allowed.
        "@UAGENT_CONFIG_SERVER_QUEUE_MAX_SIZE@": "32000",  # Maximum server's queues size.
        "@UAGENT_CONFIG_CLIENT_DEAD_TIME@": "30000",  # Client dead time in milliseconds.
        "@UAGENT_SERVER_BUFFER_SIZE@": "65535",  # Server buffer size.
    },
    template = "include/uxr/agent/config.hpp.in",
)

ros2_cpp_library(
    name = "micro_xrce_dds_agent",
    srcs = SRCS + glob(["src/cpp/**/*.h"]),
    hdrs = glob([
        "include/uxr/agent/**/*.hpp",
        "include/uxr/agent/**/*.h",
    ]) + [":config_hpp"],
    includes = ["include"],
    linkopts = select(
        {
            "@platforms//os:linux": ["-lpthread", "-lrt", "-ldl"],
            "@platforms//os:macos": ["-lpthread", "-lrt", "-ldl"],
            "@platforms//os:qnx": ["-lrt", "-ldl"],
        },
        no_match_error = "Only Linux, macOS and QNX are supported!",
    ),
    visibility = ["//visibility:public"],
    deps = [
        "@asio",
        "@tinyxml2",
        "@fmt",
        "@spdlog",
        "@fastcdr",
        "@fastrtps",
    ],
)
