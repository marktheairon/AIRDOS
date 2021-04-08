from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='airdos',
            executable='routeservice',
            name='routeservice_server',
            output='screen'),
            
        Node(
            package='airdos',
            executable='delivery_operator',
            name='delivery_operator',
            output='screen' ),
        Node(
            package='airdos',
            executable='waypoint_injector_client',
            name='waypoint_injector_client',
            output='screen' ),
        Node(
            package='ros1_bridge',
            executable='dynamic_bridge',
            name='ros1_bridge',
            output='screen'),
        ])
        
        

