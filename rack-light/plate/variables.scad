$fn = 50;

plate_width = 88.5;
plate_height = 88.5;
plate_thickness = 2.4;

hole_distance = plate_width / 2;
hole_side_distance = hole_distance / 2;
hole_height_distance = 76;
hole_z = (plate_height - hole_height_distance) / 2;

hole_diameter = 4.5;

back_ear_height = 65;
back_ear_depth = 90;
back_ear_z = (plate_height - back_ear_height) / 2;
back_ear_x = 0;

switch_hole_diameter = 6.5;

pcb_m3_hole_distance_width = 75;
pcb_m3_hole_distance_depth = 55;

m3_insert_hole_diameter = 4;
m3_insert_hole_depth = 5.75;
m3_insert_outer = m3_insert_hole_diameter + 4;

module
m3_pillar()
{
    translate([ -m3_insert_outer / 2, -m3_insert_outer / 2, -plate_thickness ])
        cube([ m3_insert_outer, m3_insert_outer, plate_thickness ]);
    difference()
    {
        translate([ -m3_insert_outer / 2, -m3_insert_outer / 2, 0 ])
            cube([ m3_insert_outer, m3_insert_outer, m3_insert_hole_depth ]);
        translate([ 0, 0, 0.001 ])
            cylinder(h = m3_insert_hole_depth, d = m3_insert_hole_diameter);
    }
}
