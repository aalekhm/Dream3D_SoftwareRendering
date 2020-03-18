Idea is to Traverse through the Control Points
@ = Original Control Points (A, B, C, D) - N control points
The Bezier Curve should pass through all @'s

								(*)            (0)
								  ,         ,
								   ,    ,
								B	@
							   .     .
						   .          .
					   .               .					D	@
				A	@					.					 .
										 .				 .
										  .			 .
										   .     .
										C	@

		For The Bezier curve that starts from 'A' & pass through 'B',
		'A' & 'B' should be the 1st & Last control points of the Bezier curve.
		We generate 4 intermediate Control Points as follows:
		#1	-	Pt1 :	'A'

		#2	-	Pt2 :	If (Start Point - A)
							then 'A'
						Else
							-Vector(A - B)
							Negative vector from B to A.
							Normalize it.
							Multiply it with some XFactor(say 5 or 10) &
							Add it to B(Point3) to get a point away from 'B' in the direction of the generated vector.
							That becomes our (0) in the figure above.

		#3	-	Pt3 :	If (Last point - D)
							then 'D'
						Else
							-Vector(C - B)
							Negative vector from B to C.
							Normalize it
							Multiply it with some XFactor(say 5 or 10) &
							Add it to B(Point3) to get a point away from 'B' in the direction of the generated vector.
							That becomes our (*) from the figure above.

		#4	-	Pt4 :	B

		#5	-	Add these 4 points to a vector.
		#6	-	Take the next control point 'B' from the original list to generate a Bezier Curve from 'B' to 'C'
				&
				Continue the same process from #1 - #5 till you reach (N - 1)th control point.

		NB: 'XFactor' forces the curvature towards the target point (less the value, straighter is the curve).
		This can be applied to 3D points to generate a smooth curve that passes through all the points in a list.