#include "PhysicsNode.h"
#include "PhysicsEngine.h"


void PhysicsNode::IntegrateForVelocity(float dt)
{
	// Apply Gravity
	// Technically gravity here is calculated by formula :
    // ( gravity / invMass * invMass * dt )
	// So even though the divide and multiply cancel out , we still
	// need to handle the possibility of divide by zero .

	if (invMass > 0.0f)
	linVelocity += PhysicsEngine::Instance() -> GetGravity() * dt;
	

	// Semi - Implicit Euler Intergration
	// - See " Update Position " below
	linVelocity += force * invMass * dt;
	
		
	// Apply Velocity Damping
	// - This removes a tiny bit of energy from the simulation each update
	// to stop slight calculation errors accumulating and adding force
	// from nowhere .
	// - In it ¡¯s present form this can be seen as a rough approximation
	// of air resistance , albeit ( wrongly ?) making the assumption that
	// all objects have the same surface area .
	linVelocity =
		linVelocity * PhysicsEngine::Instance() -> GetDampingFactor();	// Angular Rotation
	// - These are the exact same calculations as the three lines above ,
	// except for rotations rather than positions .
	// - Mass -> Torque
	// - Velocity -> Rotational Velocity
	// - Position -> Orientation
		angVelocity += invInertia * torque * dt;
	
	// Apply Velocity Damping
	angVelocity =
	angVelocity * PhysicsEngine::Instance() -> GetDampingFactor();
}

/* Between these two functions the physics engine will solve for velocity
   based on collisions/constraints etc. So we need to integrate velocity, solve 
   constraints, then use final velocity to update position. 
*/

void PhysicsNode::IntegrateForPosition(float dt)
{
	/* TUTORIAL 2 CODE */
	// Update Position
	// - Euler integration ; works on the assumption that linearvelocity
	// does not change over time (or changes so slightly it doesnt make
	// a difference ).
	// - In this scenario , gravity / will / be increasing velocity over
    // time . The in - accuracy of not taking into account of these changes
	// over time can be visibly seen in tutorial 1.. and thus how better
	// integration schemes lead to better approximations by taking into
	// account of curvature .
		position += linVelocity * dt;

	// Update Orientation
	// - This is slightly different calculation due to the wierdness
	// of quaternions . It does the same thing as position update
	// ( with a slight error ) but from i¡¯ve seen , is generally the best
	// way to update orientation .
	orientation = orientation +
	  Quaternion(angVelocity * dt * 0.5f, 0.0f) * orientation;

	// invInertia = invInertia *
	// ( Quaternion ( angVelocity * dt * 0.5f, 0.0 f)
	// * orientation ). ToMatrix3 ();
	// As the above formulation has slight approximation error , we need
	// to normalize our orientation here to stop them accumulating
	// over time .
	orientation.Normalise();

	//Finally: Notify any listener's that this PhysicsNode has a new world transform.
	// - This is used by GameObject to set the worldTransform of any RenderNode's. 
	//   Please don't delete this!!!!!
	FireOnUpdateCallback();
}