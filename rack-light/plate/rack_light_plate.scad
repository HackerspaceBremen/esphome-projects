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
    }
    translate([ 0, plate_thickness, back_ear_z ])
        cube([ plate_thickness, back_ear_depth, back_ear_height ]);
}
