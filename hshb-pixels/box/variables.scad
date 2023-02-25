$fn = 50;

box_width = 120;
box_depth = 70;
box_height = 75;
box_thickness = 2;
box_front_thickness = 7;
double_thickness = 2 * box_thickness;

lid_thickness = 2;

outer_box_width = box_width + double_thickness;
outer_box_depth = box_depth + box_thickness + box_front_thickness;
outer_box_height = box_height + box_thickness;

m3_hole_distance_width = 75;
m3_hole_distance_depth = 55;
m3_hole_wall_distance = 5;

m3_insert_hole_diameter = 4;
m3_insert_hole_depth = 5.75;
m3_insert_outer = m3_insert_hole_diameter + 4;

upper_z = box_height - m3_insert_hole_depth;
upper_m3_holes_inset = 20;
upper_m3_holes_inset_front = m3_insert_outer/2;

usb_cable_hole_width = 7;
usb_cable_hole_height = 2;
usb_cable_hole_y = m3_insert_hole_depth + 6;
usb_cable_hole_radius = 2;

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

display_pcb_width = 99;
display_pcb_depth = 1.6;
display_pcb_height = 60;
display_pcb_hole_diameter = 3.6;
display_pcb_hole_distance_x = 93;
display_pcb_hole_distance_z = 55;
display_pcb_hole_length = 35;

display_pcb_contacts_x = 6;
display_pcb_contacts_z = -2;
display_pcb_contacts_width = 48;
display_pcb_contacts_depth = 7;
display_pcb_contacts_height = 10;
display_pcb_contacts_front_depth = 5;

display_pcb_hole_distance_x_abs = (display_pcb_width-display_pcb_hole_distance_x)/2;
display_pcb_hole_distance_z_abs = (display_pcb_height-display_pcb_hole_distance_z)/2;

display_width = 98;
display_depth = 11;
display_height = 41;

module display() {
    cube([display_pcb_width, display_pcb_depth, display_pcb_height]);

    translate([display_pcb_hole_distance_x_abs,display_pcb_hole_length/2,display_pcb_hole_distance_z_abs]) rotate([90,0,0]) cylinder(h=display_pcb_hole_length,d=display_pcb_hole_diameter);

    translate([display_pcb_hole_distance_x_abs + display_pcb_hole_distance_x,display_pcb_hole_length/2,display_pcb_hole_distance_z_abs]) rotate([90,0,0]) cylinder(h=display_pcb_hole_length,d=display_pcb_hole_diameter);

    translate([display_pcb_hole_distance_x_abs,display_pcb_hole_length/2,display_pcb_hole_distance_z_abs + display_pcb_hole_distance_z]) rotate([90,0,0]) cylinder(h=display_pcb_hole_length,d=display_pcb_hole_diameter);

    translate([display_pcb_hole_distance_x_abs + display_pcb_hole_distance_x,display_pcb_hole_length/2,display_pcb_hole_distance_z_abs + display_pcb_hole_distance_z]) rotate([90,0,0]) cylinder(h=display_pcb_hole_length,d=display_pcb_hole_diameter);


    translate([(display_pcb_width-display_width)/2, display_pcb_depth-0.001, (display_pcb_height-display_height)/2]) cube([display_width, display_depth, display_height]);

    translate([
        (display_pcb_width-display_pcb_contacts_x-display_pcb_contacts_width),
        -display_pcb_contacts_depth,
        display_pcb_height-display_pcb_contacts_z-display_pcb_contacts_height
    ]) cube([
        display_pcb_contacts_width,
        display_pcb_contacts_front_depth+display_pcb_depth+display_pcb_contacts_depth,
        display_pcb_contacts_height
    ]);
}

module lid() {
    difference() {
        translate([-box_thickness,-box_thickness,0]) cube([outer_box_width, outer_box_depth, lid_thickness]);
        translate([upper_m3_holes_inset, m3_insert_outer/2, 0]) m3_hole();
        translate([box_width-upper_m3_holes_inset, m3_insert_outer/2, 0]) m3_hole();
        translate([upper_m3_holes_inset_front, box_depth-m3_insert_outer/2, 0]) m3_hole();
        translate([box_width-upper_m3_holes_inset_front, box_depth-m3_insert_outer/2, 0]) m3_hole();

        translate([box_width - 10, 20, -lid_thickness]) cylinder(h=3*lid_thickness, d=8);
        translate([box_width - 10, 11, -lid_thickness]) cylinder(h=3*lid_thickness, d=5);
    }
}
