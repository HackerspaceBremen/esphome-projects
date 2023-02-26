include <variables.scad>

union()
{
    difference()
    {
        translate([ -box_thickness, -box_thickness, -box_thickness ])
            cube([ outer_box_width, outer_box_depth, outer_box_height ]);
        cube([ box_width, box_depth, box_height + 0.001 ]);

        translate([
            -box_thickness,
            m3_hole_wall_distance + m3_hole_distance_depth / 2,
            usb_cable_hole_y + usb_cable_hole_height / 2
        ])
        {
            minkowski()
            {
                cube(
                    [
                        3 * box_thickness,
                        usb_cable_hole_width,
                        usb_cable_hole_height
                    ],
                    true);
                translate([ -box_thickness, 0, 0 ]) rotate([ 0, 90, 0 ])
                    cylinder(r = usb_cable_hole_radius, h = 3 * box_thickness);
            }
        }

        for (x = [8.5:rack_hole_he:(box_width - 5)]) {
            for (xi = [0:2]) {
                translate([
                    x + xi * rack_hole_distance_big,
                    box_depth - 10,
                    -box_thickness - 0.01
                ]) cylinder(h = 2 * box_thickness, d = rack_hole_diameter);
            }
        }
    }

    translate([ m3_hole_wall_distance, m3_hole_wall_distance, 0 ]) m3_pillar();
    translate([ upper_m3_holes_inset, m3_insert_outer / 2, 0 ]) m3_pillar_top();

    translate([
        m3_hole_wall_distance + m3_hole_distance_width,
        m3_hole_wall_distance,
        0
    ]) m3_pillar();
    translate([ box_width - upper_m3_holes_inset, m3_insert_outer / 2, 0 ])
        m3_pillar_top();

    translate([
        m3_hole_wall_distance,
        m3_hole_wall_distance + m3_hole_distance_depth,
        0
    ]) m3_pillar();
    translate([ upper_m3_holes_inset, box_depth - m3_insert_outer / 2, 0 ])
        rotate([ 0, 0, 180 ]) m3_pillar_top();

    translate([
        m3_hole_wall_distance + m3_hole_distance_width,
        m3_hole_wall_distance + m3_hole_distance_depth,
        0
    ]) m3_pillar();
    translate([
        box_width - upper_m3_holes_inset,
        box_depth - m3_insert_outer / 2,
        0
    ]) rotate([ 0, 0, 180 ]) m3_pillar_top();

    // Check if the lid holes match the box holes=
    //    translate([0,0,box_height]) lid();
}
