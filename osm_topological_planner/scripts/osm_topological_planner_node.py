#!/usr/bin/env python

import rospy
import actionlib

from OBL import OSMBridge, PathPlanner
from actionlib import SimpleActionServer
from osm_planner_msgs.msg import *
from osm_topological_planner.osm_topological_planner_callback import OSMTopologicalPlannerCallback

PACKAGE = 'osm_topological_planner'
NODE = 'osm_topological_planner_node'

class OSMTopologicalPlannerNode(object):

    def __init__(self):
        server_ip = rospy.get_param('~overpass_server_ip')
        server_port = rospy.get_param('~overpass_server_port')
        ref_lat = rospy.get_param('~ref_latitude')
        ref_lon = rospy.get_param('~ref_longitude')
        building = rospy.get_param('~building')
        global_origin = [ref_lat, ref_lon]

        rospy.loginfo("Server " + server_ip + ":" + str(server_port))
        rospy.loginfo("Global origin: " + str(global_origin))
        rospy.loginfo("Starting servers...")


        self.osm_topological_planner_server = SimpleActionServer(
            '/osm_topological_planner', OSMTopologicalPlannerAction, self._osm_topological_planner, False)
        self.osm_topological_planner_server.start()

        osm_bridge = OSMBridge(
            server_ip=server_ip,
            server_port=server_port,
            global_origin=global_origin,
            coordinate_system="cartesian",
            debug=False)
        path_planner = PathPlanner(osm_bridge)
        path_planner.set_building(building)
        self.osm_topological_planner_callback = OSMTopologicalPlannerCallback(
            osm_bridge, path_planner)
        rospy.loginfo(
            "OSM topological planner server started. Listening for requests...")

    def _osm_topological_planner(self, req):
        res = self.osm_topological_planner_callback.get_safe_response(req)
        if res is not None:
            self.osm_topological_planner_server.set_succeeded(res)
        else:
            self.osm_topological_planner_server.set_aborted(res)


if __name__ == "__main__":
    rospy.init_node(NODE)
    osm_topological_planner_node = OSMTopologicalPlannerNode()
    rospy.spin()
