/**
 * Note: Do not include any external libraries.
 * You may only use the drawing functions provided to complete this assignment.
 */

/**    HELPER FUNCTIONS     **/
/** Do not modify this code **/

/**
 * Draws a line segment on the screen connecting (x1, y1) and (x2, y2). This is the only function
 * you may call to draw.
 * @param  {DOMString} ctx - Canvas context to draw to. Simply pass in the given ctx variable
 * @param  {number} x1 - X coordinate of the start point
 * @param  {number} y1 - Y coordinate of the start point
 * @param  {number} x2 - X coordinate of the end point
 * @param  {number} y2 - Y coordinate of the end point
 * @param  {String} style - A string giving the colour of the line. Default is black.
 * @return {undefined}
 */
function drawLineSegment(ctx, x1, y1, x2, y2, style= "black")
{
  ctx.beginPath();
  ctx.moveTo(x1,y1);
  ctx.lineTo(x2,y2);
  ctx.strokeStyle = style;
  ctx.stroke();
}

/**
 * Takes a point defined as an array of numbers [x, y] and treats it as a homogeneous coordinate [x, y, 1]
 * The transformation T is a 2x3 matrix defined as an array of arrays
 * T = [[T11, T12, T13],
 *      [T21, T22, T23]]
 * The result is a new transformed point [x', y'] = T * [p[0], p[1], 1]
 * @param  {array} p - Point to transform
 * @param  {array} T - Transformation to apply
 * @return {array} q ->New transformed point which is an array contained 2 parameters in 1x2 form
 */
function transformPoint(p,T)
{
    var q = new Array();

  for(var i = 0; i<T.length; i++)
  {
    q[i] = 0;
    for(var j = 0; j<p.length; j++)
      q[i] += T[i][j] * p[j];
	// Translation
	q[i] += T[i][j];
  }
  
  return q;
}

/**
 * Given two transformations T1 and T2, returns a new transformation equivalent to applying
 * T1 followed by T2.
 * @param  {array} T1 - The first transform to apply
 * @param  {array} T2 - The second transfrom to apply
 * @return {array} A new transformation
 */
function composeTransforms(T1, T2) {
  var T_new = [
    [0, 0, 0],
    [0, 0, 0]
  ];

  for (var i = 0; i < 2; i++) {
    for (var j = 0; j < 3; j++) {
      for(var k = 0; k < 3; k++) {
        T_new[i][j] += T2[i][k] * (k === 2 ? (j === 2 ? 1 : 0) : T1[k][j]);
      }
    }
  }

  return T_new;
}
/** END OF HELPER FUNCTIONS **/


/**
 * This function should take a list of points and draw a closed polygon connecting the points.
 * Each point is an array containing two numbers [x, y].
 * @param  {DOMString} ctx - Canvas context to draw to. Simply pass in the given ctx variable
 * @param  {array} poly - Array of points corresponding to the vertices of the polygon [p1, p2, p3, ..., pn]
 * @return {undefined}
 */
function drawPolygon(ctx, poly)
{
  /** Fill in your answer here **/
  // Hint: call drawLineSegment to help you
 //The for loop has the function to draw line by the adjacent points and make it connected
  for(var i=0;i<poly.length-1;i++){
      drawLineSegment(ctx,poly[i][0],poly[i][1],poly[i+1][0],poly[i+1][1],style= "black");
  }
    var i=poly.length -1;
    drawLineSegment(ctx,poly[i][0],poly[i][1],poly[0][0],poly[0][1],style= "black");
    return;
}
/**
 * Given an array of points defining a polygon, return a new array of points for a polygon transformed by
 * the transformation T.
 * @param  {array} poly - Array of points corresponding to the vertices of the polygon [p1, p2, p3, ..., pn]
 * @param  {array} T - T transformation to apply. Same definition as in transformPoint()
 * @return {array} Array of vertex locations for the transformed polygon
 */
function transformPolygon(poly, T)
{
  /** Fill in your answer here **/
  // Hint: call transformPoint to help you
  var q = new Array();
//This nested for loop is make an array of array which is also a nx2 matrix where n depends on the number of //the poly size 
 for(var i=0;i<poly.length;i++){
     q[i]=new Array();
     for(var j=0; j<2;j++){
         q[i][j]=0;
     } 
 }
// Array V is created to store the result from transformPoint(p,T)
    var v = new Array();
//now call transformPoint helper function to calculate the point and store in the q[i][j]
  for(var i=0;i<poly.length;i++){
      //point return from the helper function is an array contained 2 elements v[0]=x' v[1]=y'
      v=transformPoint(poly[i],T);
      for(var j=0;j<2;j++){
          q[i][j]=v[j];              
      }
  }
  return q;
}

/**
 * Draws a circle of radius r around point [cx, cy].
 * @param  {DOMString} ctx - Canvas context to draw to. Simply pass in the given ctx variable
 * @param  {number} cx - X coordinate of the circle center
 * @param  {number} cy - Y coordinate of the circle center
 * @param  {number} r - Radius of the circle
 * @return {undefined}
 */
function drawCircle(ctx, cx, cy, r)
{
  /** Fill in your answer here **/
  // Hint: call drawLineSegment to help you
 //function drawLineSegment(ctx, x1, y1, x2, y2, style= "black")
    var x = Array();
    var y = Array();
    for(var i =0; i<60;i++){
        x[i]= cx + r*(Math.cos(i*Math.PI/30));
        y[i]= cy + r*(Math.sin(i*Math.PI/30));
    }
    for (var i=0; i<59;i++){
        drawLineSegment(ctx,x[i],y[i],x[i+1],y[i+1],style= "black");
    }
    drawLineSegment(ctx,x[59],y[59],x[0],y[0],style= "black");
    return;
}

/**
 * Returns a transformation matrix T that defines a translation in homogeneous coordinates
 * by the vector offset [x, y].
 * That is, if applied to a point p = [p1, p2, 1] then it will be
 * translated to p' = [p1 + x, p2 + y, 1]
 * @param  {array} offset - The offset to translate by
 * @return {array} A 2x3 transformation matrix
 */
function translateByOffset(offset)
{
  var T = [
    [1, 0, 0],
    [0, 1, 0]
  ];

  /** Fill in your answer here **/
//This translation function can be hard-coded since the only thing we neee to change is t_x and t_y.
  T[0][2]=offset[0];//offset[0]=x
  T[1][2]=offset[1];//offset[1]=y
  
  return T;
}

/**
 * Returns a transformation matrix T that defines
 * a rotation by angle radians around a point joint.
 * @param  {number} angle - Angle in radians
 * @param  {array} joint - point [x, y] to rotate around 
 * @return {array} T - 2x3 transformation matrix defining the rotation
 */
function rotationAboutPoint(angle, joint)
{
  var Translation = [
    [1, 0, 0],
    [0, 1, 0]
  ];
  /** Fill in your answer here **/
  // Hint: Use Math.cos() and Math.sin()
 //rotation around a point joint, which means we need to translate the matrix to set the joint point as origin //and do the rotation and undo the translation
 //first we need to call the helper function translateByOffset
    var Translation = translateByOffset(joint);
    
    Translation_back=Translation; //now it set the joint to the origin
    Translation_back[0][2]=-joint[0];//offset[0]=x
    Translation_back[1][2]=-joint[1];//offset[1]=y
    //now we need to do the rotation about the joint point by the angle
    //since in this definition of rotation matrix needed to be formed in 2X3 
    var Rotate =[
        [Math.cos(-angle), -Math.sin(-angle),0],
        [Math.sin(-angle), Math.cos(-angle),0]
    ];
    //create a mid-step matrix to store the value of transformation of Translation and Rotation

    var TR = composeTransforms(Translation_back,Rotate);
    //TR = composeTransforms(Translation,Rotate);
    //since we set the origin to the joint, therefore we need to set it back after rotation
  
    //M stands for Matrix where M= (T^-1)*R*T

    var M = composeTransforms(TR, Translation);
    
  return M;
}


function drawPenguin(ctx)
{
  /** GET UI SLIDER VARIABLES **/
  /** You do not need to change this code **/
  var deg_to_rad = Math.PI / 180.0;
  var torso_x_offset = parseFloat(document.getElementById("slider_torso_x").value);
  var torso_y_offset = parseFloat(document.getElementById("slider_torso_y").value);
  var arm_angle = parseFloat(document.getElementById("slider_arm_angle").value) * deg_to_rad;
  var head_angle = parseFloat(document.getElementById("slider_head_angle").value) * deg_to_rad;
  var mouth_gap = parseFloat(document.getElementById("slider_mouth_gap").value);
  var hip_angles = [
    parseFloat(document.getElementById("slider_hip_angle0").value) * deg_to_rad,
    parseFloat(document.getElementById("slider_hip_angle1").value)  * deg_to_rad
  ];
  var ankle_angles = [
    parseFloat(document.getElementById("slider_ankle_angle0").value)  * deg_to_rad,
	parseFloat(document.getElementById("slider_ankle_angle1").value)  * deg_to_rad
  ];
  /** END OF GET UI SLIDER VARIABLES **/


  /** POLYGON DEFINITIONS **/
  /**
   * The polygons defining each body region are provided *relative to the origin*
   * It is your responsibility to transform and draw these polygons to construct a hierarchical model
   * that can be manipulated by the sliders. 
   *
   * You may not change these values manually.
   */
  // 
  var torso_poly = [[-56, -196], [-129, 99], [-46, 188], [36, 196], [129, 116], [69, -71], [36, -196]];
  var arm_poly = [[-38, -78], [-15, 78], [22, 78], [39, -74]];

  var head_poly = [[-19, -50], [-49, -29], [-62, 50], [62, 48], [46, -27]];
  var upper_beak_poly = [[41, -13], [-44, 2], [-41, 13], [43, 13]];
  var lower_beak_poly = [[-40, -3], [-42, 3], [41, 3], [41, -3]];
  
  var leg_poly = [[-15, -53], [-20, 52], [ 20,  52], [ 12, -53]];
  var foot_poly = [[54, -11], [58, 11], [-48,  11], [-58, -11]];
  
  // These will be drawn as circles
  var eye = [0, 0];
  var iris = [0, 0];
  
  // Radii of circles
  var eye_r = 10;
  var iris_r = 3;
  var arm_joint_r = 10;
  var head_joint_r = 5;
  var hip_joint_r = 5;
  var ankle_joint_r = 5;
  
  // Joint positions
  var hip_joints = [[0, 0], [0, 0]];
  var ankle_joints = [[0, 0], [0, 0]];
  var arm_joint = [0, 0];
  var head_joint = [0, 0];
  /** END OF POLYGON DEFINITIONS **/


  /*********************************/
  /** YOUR DRAWING CODE GOES HERE **/
  /*********************************/
 //torso, head, arm, upper_beck, lower_beck, leg, foot, eye, iris, mouth
  /** The torso and head are given as examples **/
  // Define the center of the torso in screen space
  // and a transformation to translate the torso to its origin
  var torso_origin = [360, 360];
  var torso_T = translateByOffset([torso_origin[0] + torso_x_offset, torso_origin[1] + torso_y_offset]);
  torso_poly = transformPolygon(torso_poly, torso_T); // translate the point from origin to torso's origin
  drawPolygon(ctx, torso_poly);

  // Define the transformation for the head as a rotation and then a translation
  var head_offset = [-10, -150];// The location of the head relative to the body
  var head_joint_offset = [0, 35]; // The location of the head joint relative to the head center
  var head_T = composeTransforms(
    rotationAboutPoint(head_angle, head_joint_offset),// first apply the rotation around the point
    translateByOffset(head_offset)//then second apply the translation
  );
 
  // Transform and draw the head in a hierarchical fashion
  // That is, if the body moves, then the head will move with it.
  head_poly = transformPolygon(head_poly, head_T);//
  head_poly = transformPolygon(head_poly, torso_T);//head will change its position when torso changes  
  drawPolygon(ctx, head_poly);

  // Now draw the head joint
  head_joint = transformPoint(head_joint_offset, head_T);
  head_joint = transformPoint(head_joint, torso_T);
  drawCircle(ctx, head_joint[0], head_joint[1], head_joint_r);

   /* Draw the rest of the penguin below. */
   //first draw the iris and next draw the eye
    //Define the transformation for the iris as a rotation and then a translation
    var iris_offset = [-35,-20];//The location of the iris relative to the head center
    //Define the transformation of the iris as a translation
    var iris_T = translateByOffset(iris_offset);
     // Transform and draw the head in a hierarchical fashion
    //if head moves, then the iris will move with it
   // That is, if the body moves, then the head will move with it.
    iris = transformPoint(iris,iris_T);
    iris = transformPoint(iris,head_T);
    iris = transformPoint(iris, torso_T);
    drawCircle(ctx,iris[0],iris[1],iris_r);
    
    /*var eye_offset = [-35,-20];
    var eye_T = translateByOffset(eye_offset);
    eye = transformPoint(eye,eye_T);
    eye = transformPoint(eye,head_T);
    eye = transformPoint(eye, torso_T);*/
    //since eye is same as iris except the radius of the eye
    drawCircle(ctx,iris[0],iris[1],eye_r);
    
    //Define the transformation for the arm as the rotation and then a translation

    var arm_offset= [20, -125];// The location of the arm relative to the body
    var arm_joint_offset = [0, -50];//The location of the arm joint relative to the arm center
    var arm_T = composeTransforms(
         rotationAboutPoint(arm_angle,arm_joint_offset),
         translateByOffset(arm_offset)
    );
    arm_poly = transformPolygon(arm_poly,arm_T);
    //arm_poly= translateByOffset(arm_offset);
    arm_poly = transformPolygon(arm_poly, torso_T); 
    drawPolygon(ctx,arm_poly);
    
     // Now draw the arm joint
    arm_joint = transformPoint(arm_joint_offset,arm_T);
    arm_joint = transformPoint(arm_joint, torso_T);
    drawCircle(ctx,arm_joint[0],arm_joint[1],arm_joint_r);
    
    
    //Define the transformation for the upper beak as the translation
    var upper_beak_offset = [-90,5];//The location of the upper_beak relative to the head
    var upper_beak_T = translateByOffset(upper_beak_offset);
    /*
    since we want the upper_beak relative to the head within rotation and translation, therefore we can transform the polygon as below. First, we transform the poly in our upper_beak coordinates and then transform in head coordinate and finally change to the torso coordinate to the global coordinate.
    */
     upper_beak_poly = transformPolygon(upper_beak_poly,upper_beak_T);
     upper_beak_poly = transformPolygon(upper_beak_poly,head_T);
     upper_beak_poly = transformPolygon(upper_beak_poly,torso_T);
     drawPolygon(ctx,upper_beak_poly);
    
    
    //Define the transformation for the lower_beak  as the translation
    //mouth_gap which apply the lower_beak to tranlate to the upper_beak
     var lower_beak_offset =[-90,30+mouth_gap];
     var lower_beak_T = translateByOffset(lower_beak_offset);

     lower_beak_poly = transformPolygon(lower_beak_poly,lower_beak_T);
     lower_beak_poly = transformPolygon(lower_beak_poly,head_T);
     lower_beak_poly = transformPolygon(lower_beak_poly,torso_T);
     drawPolygon(ctx,lower_beak_poly);
    
    //Define the transformation of the leg as the rotation and then translation
    var leg_offset_0 = [-50,110]; //The location of leg relative to the body
    var hip_joint_offset = [0,-40];//The location of the joint relative to the leg center
    var hip0_T = composeTransforms(
          rotationAboutPoint(hip_angles[0],hip_joint_offset),
          translateByOffset(leg_offset_0)
    );
    var hip_poly_0 = leg_poly;
    hip_poly_0 = transformPolygon(hip_poly_0,hip0_T);
    hip_poly_0 = transformPolygon(hip_poly_0,torso_T);
    drawPolygon(ctx,hip_poly_0);
    
    //Now draw the hip_joint[0]
    hip_joints[0] = transformPoint(hip_joint_offset,hip0_T);
    hip_joints[0] = transformPoint(hip_joints[0], torso_T); 
    drawCircle(ctx,hip_joints[0][0],hip_joints[0][1],hip_joint_r);
    
    //Define the transformation of the leg as the rotation and then translation
    var leg_offset_1 = [60,110]; //The location of leg relative to the body
    var hip1_T = composeTransforms(
          rotationAboutPoint(hip_angles[1],hip_joint_offset),
          translateByOffset(leg_offset_1)
    );
    var hip_poly_1 = leg_poly;
    hip_poly_1 = transformPolygon(hip_poly_1,hip1_T);
    hip_poly_1 = transformPolygon(hip_poly_1,torso_T);
    drawPolygon(ctx,hip_poly_1);
    
    //Now draw the hip_joint[1]
    hip_joints[1] = transformPoint(hip_joint_offset,hip1_T);
    hip_joints[1] = transformPoint(hip_joints[1], torso_T); 
    drawCircle(ctx,hip_joints[1][0],hip_joints[1][1],hip_joint_r);
    
    //Define the transformation for the ankle as the rotation and the translation
    var foot_offset_0 = [50,50];//[-50,30]The location of the foot relative to the leg
    var foot_joint_offset_0 = [45,5]; //The location of the foot relative to the foot center
    var foot_T_0 = composeTransforms(
        rotationAboutPoint(ankle_angles[0],foot_joint_offset_0),
        translateByOffset(foot_offset_0)
    );

    var foot_poly_0 = transformPolygon(foot_poly, foot_T_0);
    foot_poly_0 = transformPolygon(foot_poly_0,hip0_T);
    foot_poly_0 = transformPolygon(foot_poly_0, torso_T);
    drawPolygon(ctx,foot_poly_0);
    
    ankle_joints[0] = transformPoint(foot_joint_offset_0,foot_T_0);
    ankle_joints[0] = transformPoint(ankle_joints[0],hip0_T);
    ankle_joints[0] = transformPoint(ankle_joints[0], torso_T); 
    drawCircle(ctx,ankle_joints[0][0],ankle_joints[0][1],ankle_joint_r);
    ///////////////////////////////////// 
    var foot_offset_1 = [50,50];//[-50,30]The location of the foot relative to the leg
    var foot_joint_offset_1 = [45,5]; //The location of the foot relative to the foot center
    var foot_T_1 = composeTransforms(
        rotationAboutPoint(ankle_angles[1],foot_joint_offset_1),
        translateByOffset(foot_offset_1)
    );

    var foot_poly_1 = transformPolygon(foot_poly, foot_T_1);
    foot_poly_1 = transformPolygon(foot_poly_1,hip1_T);
    foot_poly_1 = transformPolygon(foot_poly_1, torso_T);
    drawPolygon(ctx,foot_poly_1);
    
    ankle_joints[1] = transformPoint(foot_joint_offset_1,foot_T_0);
    ankle_joints[1] = transformPoint(ankle_joints[1],hip1_T);
    ankle_joints[1] = transformPoint(ankle_joints[1], torso_T); 
    drawCircle(ctx,ankle_joints[1][0],ankle_joints[1][1],ankle_joint_r);
}

/**        BOILERPLATE DRAWING CODE        **/
/**   You do not need to modify this code  **/
function drawAxis(ctx)
{
  drawLineSegment(ctx, 0, 0, 100, 0, "red");
  drawLineSegment(ctx, 0, 0, 0, 100, "green");
  ctx.fillText("0,0", 4, 12);
  ctx.fillText("100,0", 98, 10);
  ctx.fillText("0,100", 5, 100);
}

// This function is called to draw the current frame. It is called whenever the
// sliders are changed.
function draw() 
{
  // you do not need to modify this function
  // this is help function
  // Boilerplate code to setup the canvas
  var canvas = document.getElementById('canvas');
  if (!canvas.getContext)
  {
    alert("Your browser is too old! Please upgrade to a canvas-capable browser.");
  }
  var ctx = canvas.getContext('2d');
  // clear canvas so we can draw a new frame
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  // set global line width and smooth lines
  ctx.lineWidth = 2;
  ctx.lineJoin = 'round';
  ctx.lineCap = 'round';
  // draw a small axis to demonstrate the inverted coordinate system
  drawAxis(ctx);
  // draw the current Penguin
  drawPenguin(ctx);
}
/**     END OF BOILERPLATE DRAWING CODE       **/