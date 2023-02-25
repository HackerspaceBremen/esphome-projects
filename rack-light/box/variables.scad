$fn = 50;

box_width = 95;
box_depth = 85;
box_height = 40;
box_thickness = 2;
double_thickness = 2 * box_thickness;

lid_thickness = 2;

outer_box_width = box_width + double_thickness;
outer_box_depth = box_depth + double_thickness;
outer_box_height = box_height + box_thickness;

m3_hole_distance_width = 75;
m3_hole_distance_depth = 55;
m3_hole_wall_distance = 5;

m3_insert_hole_diameter = 4;
m3_insert_hole_depth = 5.75;
m3_insert_outer = m3_insert_hole_diameter + 4;

upper_z = box_height - m3_insert_hole_depth;
upper_m3_holes_inset = 25;

usb_cable_hole_width = 7;
usb_cable_hole_height = 2;
usb_cable_hole_y = m3_insert_hole_depth + 6;
usb_cable_hole_radius = 2;

rack_hole_diameter = 6.5;
rack_hole_distance_big = 15.88;
rack_hole_distance_small = 12.7;
rack_hole_he = 2 * rack_hole_distance_big + rack_hole_distance_small;

module m3_pillar() {
    translate([-m3_insert_outer/2,-m3_insert_outer/2,-box_thickness]) cube([m3_insert_outer,m3_insert_outer,box_thickness]);
    difference() {
        translate([-m3_insert_outer/2,-m3_insert_outer/2,0]) cube([m3_insert_outer,m3_insert_outer,m3_insert_hole_depth]);
        translate([0,0,0.001]) cylinder(h=m3_insert_hole_depth,d=m3_insert_hole_diameter);
    }
}

module m3_pillar_top() {
    translate([0,0,box_height-m3_insert_hole_depth]) {
        m3_pillar();
        translate([0,-m3_insert_outer/2,-box_thickness]) rotate([0,90,0]) linear_extrude(height=m3_insert_outer,center=true) {
            polygon([[0,0],[0,m3_insert_outer],[m3_insert_outer/1.5,0]]);
        }
    }
}

module m3_hole() {
    translate([0,0,-lid_thickness]) cylinder(h=3*lid_thickness, d=3.3);
}

module lid() {
    difference() {
        translate([-box_thickness,-box_thickness,0]) cube([outer_box_width, outer_box_depth, lid_thickness]);
        translate([upper_m3_holes_inset, m3_insert_outer/2, 0]) m3_hole();
        translate([box_width-upper_m3_holes_inset, m3_insert_outer/2, 0]) m3_hole();
        translate([upper_m3_holes_inset, box_depth-m3_insert_outer/2, 0]) m3_hole();
        translate([box_width-upper_m3_holes_inset, box_depth-m3_insert_outer/2, 0]) m3_hole();
    }
}
