include <variables.scad>

union()
{
    difference()
    {
        cube([ plate_width, plate_thickness, plate_height ]);
        translate([ hole_side_distance, -1, hole_z ])
        {
            rotate([ -90, 0, 0 ])
                cylinder(h = 2 * plate_thickness, d = hole_diameter);
            translate([ 0, 0, hole_height_distance ]) rotate([ -90, 0, 0 ])
                cylinder(h = 2 * plate_thickness, d = hole_diameter);
        }
        translate([ plate_width - hole_side_distance, -1, hole_z ])
        {
            rotate([ -90, 0, 0 ])
                cylinder(h = 2 * plate_thickness, d = hole_diameter);
            translate([ 0, 0, hole_height_distance ]) rotate([ -90, 0, 0 ])
                cylinder(h = 2 * plate_thickness, d = hole_diameter);
        }

        translate([ plate_width / 2, -1, plate_height / 2 ])
            rotate([ -90, 0, 0 ])
                cylinder(h = 2 * plate_thickness, d = switch_hole_diameter);
    }

    translate([ 0, plate_thickness, back_ear_height + back_ear_z ])
        rotate([ 0, 90, 0 ])
    {
        cube([ back_ear_height, back_ear_depth, plate_thickness ]);
        translate([ 0, 0, plate_thickness ]) cube(
            [ back_ear_height, plate_thickness * 2, m3_insert_hole_depth ]);

        translate([ back_ear_height / 2, back_ear_depth / 2, plate_thickness ])
        {
            translate([
                pcb_m3_hole_distance_depth / 2,
                pcb_m3_hole_distance_width / 2,
                0
            ]) m3_pillar();
            translate([
                pcb_m3_hole_distance_depth / 2,
                -pcb_m3_hole_distance_width / 2,
                0
            ]) m3_pillar();
            translate([
                -pcb_m3_hole_distance_depth / 2,
                pcb_m3_hole_distance_width / 2,
                0
            ]) m3_pillar();
            translate([
                -pcb_m3_hole_distance_depth / 2,
                -pcb_m3_hole_distance_width / 2,
                0
            ]) m3_pillar();
        }
    }
}
