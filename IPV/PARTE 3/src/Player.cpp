//FICHERO MODIFICADO-E2 
/*/////////////////////////////////////////////////////////////////////////////////////
//
// Player initialization or constructor
//
// These values are by default. They have to be overwritten in the initializatio phase
// when reading the default values from the "initialization.html" file

	@author Ramon Molla
	@version 2011-09-11

	*/
#include <Glee\GLee.h>

#include <Laser.h>
#include <Player.h>
#include <GameCharacters.h>
#include <SIMessage.h>
#include <ShootsManager.h>	/// Header File class Manager for the shoots
#include <GlobalDefs.h>
#include <SIKeyboard.h>
#include <SIGame.h>

#define CPL_DEFAULT_LIVES		  3 ///<If not specified, by default, ther are three lives

#define CPL_MAX_HEALTH			 20	///<Maximun amount of health of the player by default
#define CPL_MAX_MAX_HIT_DUR		500	///<Maximun amount of time when the player is hit

#define CPL_BOUNDARY_MARGIN		1.0f
#define CPL_BOUNDARY_FRICTION	0.01f
#define CPL_VISCOUS_COEFF		0.002f

#define CPL_MIN_SPEED			0.0004f	//Minimun X speed in units/ms
#define CPL_MAX_SPEED			0.010f	//Maximun X speed in units/ms
#define CPL_MIN_ACCEL			0.0002f	//Minimun X acceleration in units/ms
#define CPL_MAX_ACCEL			0.0008f	//Maximun X acceleration in units/ms

#define CPL_2D_SIZE_X			0.6f	//
#define CPL_2D_SIZE_Y			0.4f	//
#define CPL_2D_SIZE_Z			0.0f	//

#define CPL_3D_SIZE_X			1.0f	//
#define CPL_3D_SIZE_Y			2.0f	//
#define CPL_3D_SIZE_Z			0.5f	//

#define CPL_LASER_XSHIFT		0.05f	///<Left or right laser relative position respect to the player on the X axis
#define CPL_REACTOR_XSHIFT		0.23f	///<Left or right reactor relative position respect to the player on the X axis
#define CPL_REACTOR_YSHIFT		0.30f	///<Left or right reactor relative position respect to the player on the Y axis
#define CPL_REACTOR_ZSHIFT		0.0f	///<Left or right reactor relative position respect to the player on the Z axis

//Shooting
#define CPL_SHOOT2D_SPEED		0.006f	//In units per milisecond
#define CPL_SHOOT3D_SPEED		0.005f	//In units per milisecond

extern CShootsManager	*ShootsManager;
extern CSIGame			*Game;
CCharactersPool			*CharacterPool;


bool CPlayer::Init ()	//Used when all the values are initialized by default
						//when reading the global initialization game file. 
						//Returns true if the initialization of the attirbute has been correctly done
{
	Type			=	CHARS_PLAYER;
	Lives			=	CPL_DEFAULT_LIVES;
	Health			=	MaxHealth = CPL_MAX_HEALTH;
	Hit_duration	=	CPL_MAX_MAX_HIT_DUR;

	UpdateSF(TimerManager.GetSF());

	yi				=	0.00;
	yi_speed		=	0.01f;
	yi_cos			=	0;
	zi				=	0.00;
	zi_speed		=	0.01f;

	ResetScale();

	Rotation.v[XDIM] = 0.0;
	Rotation.v[YDIM] = 0.0;
	Rotation.v[ZDIM] = 180.0;

#ifdef CHAR_USE_AABB
	UpdateAABB(CPL_2D_SIZE_X, CPL_2D_SIZE_Y, CPL_2D_SIZE_Z);
#endif

	Speed.v[XDIM]			= 0.0f;
	Acceleration.v[XDIM]	= 0.0f;

	ShootType		= CSH_PLAYER2D;

	for (int r = 0; r < CPL_MAX_REACTORS; r++)
		Reactor[r] = (CReactor*) CharacterPool->get(CHARS_REACTOR, UGKOBJM_NO_SUBTYPE);
	
	for (int l = 0; l < CPL_MAX_LASERS; l++)
		Laser[l] = (CLaser*) CharacterPool->get(CHARS_LASER, UGKOBJM_NO_SUBTYPE);

	ActivateReactor();

	Passing2D23D = false;
	DiscreteSimulation = false;

	Direction	= 0.0f;

	State		= CPL_UNBORN;
	Scrolling	= 0.0f;

	/*MODIFICADOO PARTE ENTREGA2*/
		//En esta version de SI hay un bug y si dañan o matan al player peta a los 20 - 30 segs
		// Con esto, evitamos que muera y lance la excepcion
		//SOLO SE MODIFICO ESTA LINEA EN TODO EL ARCHIVO
	Immortal = true; 

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Player Initialisation 
//
//

CPlayer::CPlayer()
{
	Init();

	//Create a new default propietary msg for internal management only
	msg = new RTDESK_CMsg;
	msg->Type = UMSG_MSG_BASIC_TYPE;
	msg->Propietary = true;
}

#ifdef CHAR_USE_QUADTREE
void CPlayer::SetQTRoot(QuadtreeRoot *QTR)
{
	QTRoot = QTR;
	for (int l = 0; l<CPL_MAX_LASERS; l++)
		Laser[l]->SetQTRoot(QTR);
	
	for (int r = 0; r<CPL_MAX_REACTORS; r++)
		Reactor[r]->SetQTRoot(QTR);
}
#endif


void CPlayer::ResizeDirectory(unsigned int S)
{
	unsigned int	i;
	
	CExplosiveChar::ResizeDirectory(S);

	for (i = 0; i<CPL_MAX_LASERS; i++)
		Laser[i]->ResizeDirectory(S);

	for (int r = 0; r<CPL_MAX_REACTORS; r++)
		Reactor[r]->ResizeDirectory(S);
}

void CPlayer::SetGameRef(CCharacter *Game)
{
	unsigned int i;

	//The Directory resize
	AssignTMG(Game);

	for (i = 0; i<CPL_MAX_LASERS; i++)
		Laser[i]->AssignTMG(Game);
	for (int r = 0; r<CPL_MAX_REACTORS; r++)
		Reactor[r]->AssignTMG(Game);
}

/**
*	@fn CPlayer::SetLocalTimers()
*	Starts up all the local timers required to work during tipical phases update, render,...
*/
void CPlayer::SetLocalTimers()
{
	unsigned int i;

	//Own timers
	CExplosiveChar::SetLocalTimers(CPL_MAX_TIMERS);
	
	//Laser Timers
	for (i = 0; i < CPL_MAX_LASERS; i++)
		Laser[i]->SetLocalTimers(CPL_MAX_TIMERS);
	
	//Reactor timers
	for (int r = 0; r<CPL_MAX_REACTORS; r++)
		Reactor[r]->SetLocalTimers(CPL_MAX_TIMERS);

	SetRenderPeriod(Game->DefaultRndPeriod[CHARS_GAME]);
}

void CPlayer::AssignTextMngr(CCharacter * TM)
{
	unsigned int	i;

	CExplosiveChar::AssignTextMngr(TM);

	for (i = 0; i<CPL_MAX_LASERS; i++)
		Laser[i]->AssignTextMngr(TM);

	for (int r = 0; r<CPL_MAX_REACTORS; r++)
		Reactor[r]->AssignTextMngr(TM);
}

void CPlayer::Pass2D3D()
{
	unsigned int i;  
	
	Passing2D23D = true; 
	for (i = 0; i < CPL_MAX_LASERS; i++) 
		Laser[i]->Passing2D23D = true;
	//Reactor is always a particle system that does not need to pass from 2D to 3D. 
}

/**
*	@fn void CBonusManager::SetEndingCriteria(bool ByTime, bool ByFrame)
*	Set the conditions to finish the game
*/
void CPlayer::SetEndingCriteria(bool ByTime, bool ByFrame)
{
	unsigned int i;

	EndByTime = ByTime;
	EndByFrame = ByFrame;

	for (i = 0; i < CPL_MAX_LASERS; i++)
	{
		Laser[i]->EndByTime = ByTime;
		Laser[i]->EndByFrame = ByFrame;
	}

	for (int r = 0; r < CPL_MAX_REACTORS; r++)
	{
		Reactor[r]->EndByTime = ByTime;
		Reactor[r]->EndByFrame = ByFrame;
	}
}

//AI
void CPlayer::AI_Init	(void)	///Default Artificial Intelligence character initialization
{
	State = CPL_UNBORN;
}

void CPlayer::AI_Explode (void)
{
	Sound[CPL_TOUCH_SND]->Play(UGKSND_ULTRA_LOW_VOLUME);
	
	Explosion.Init(this, Timer[CL_RND_PERIOD].GetAlarmPeriodms());
	Explosion.Activate();
}	

void CPlayer::AI_Die(void)	///Default Artificial Intelligence character initialization
{
	State = CPL_DYING;
	RTDESK_CMsg *pMsg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
	SendMsg(pMsg, Game, RTDESKT_INMEDIATELY);
}

void CPlayer::ActivateReactor()
{
	Vector ReactorShift;

	for (int r = 0; r < CPL_MAX_REACTORS; r++)
	{
		Reactor[r]->Activate();
		Reactor[r]->MoveTo(Position);
	}

	ReactorShift.Set(-CPL_REACTOR_XSHIFT, -CPL_REACTOR_YSHIFT, CPL_REACTOR_ZSHIFT);
	Reactor[CPL_LEFT_REACTOR]->MoveRelTo(ReactorShift);
	ReactorShift.Set(CPL_REACTOR_XSHIFT, -CPL_REACTOR_YSHIFT, CPL_REACTOR_ZSHIFT);
	Reactor[CPL_RIGHT_REACTOR]->MoveRelTo(ReactorShift);		
}

void CPlayer::ActivateLaser(CPL_PLAYER_LASER LaserActivated)
{
	Laser[LaserActivated]->Activate();
	Laser[LaserActivated]->Explosion.SetDefault();
	
	if (CPL_LEFT_LASER == LaserActivated)
		Laser[LaserActivated]->Position.v[XDIM] = Position.v[XDIM] - CharAABB.AABB[CHAR_BBSIZE][XDIM].Value - CPL_LASER_XSHIFT;
	else 
		Laser[LaserActivated]->Position.v[XDIM] = Position.v[XDIM] + CharAABB.AABB[CHAR_BBSIZE][XDIM].Value + CPL_LASER_XSHIFT;

	Laser[LaserActivated]->Position.v[YDIM] = Position.v[YDIM];
	Laser[LaserActivated]->Position.v[ZDIM] = Position.v[ZDIM];

	Laser[LaserActivated]->ShootType = CSH_AUX_LASER;
	Laser[LaserActivated]->Timer[CL_RND_PERIOD].SetAlarm(Timer[CPL_RND_PERIOD].GetAlarmPeriod());

	Laser[LaserActivated]->UpdateCollisionDetection();
}

//Physics
void CPlayer::Collided (CCharacter *CollidedChar)
{
	unsigned int i;

if (Alive() && !Immortal)
	{
#ifdef DEBUG_MORTAL
		if (Passing2D23D)	return;	// Stop checking more collisions. The player is protected while changing from mode 2D to mode 3D

		Sound[CPL_TOUCH_SND]->Play(UGKSND_VOLUME_60);
#endif
	}
	else return;

	if (CHARS_COLLISION_TABLE[CHARS_PLAYER][CollidedChar->Type])
		switch (CollidedChar->Type)
		{
		case CHARS_SHOOT:
			switch (((CShoot*)CollidedChar)->SubType)
			{
			case CSH_SHIP:				///<Shoot sent by a enemy ship. This kind of shoot has to be always after all player's shootings
			case CSH_SUPPLY_SHIP:		///<Shoot sent by a enemy Supply Ship. This kind of shoot has to be always after all player's shootings
			case CSH_CIRCLE_SHIP:
				Health -= CollidedChar->Health;
				Hit_duration--;
				break;
			default:
				return;
			}
			break;
		case CHARS_SHIP:
			#ifdef DEBUG_MORTAL
			if(CHAR_2D == RenderMode)
				Health  = 0;
			else
				Health -= CollidedChar->Health;
			#endif
			Hit_duration--;
			break;
		case CHARS_BONUS:
			//These collisions are detected and managed by bonus manager when collided with the player.
			Health = Health;
			break;
		}

	if (Health <= 0)
	{	//... and everything has to explode, if exists
		for (i = 0; i < CPL_MAX_LASERS; i++)
			if (Laser[i]->Alive())
				Laser[i]->AI_Explode();

		Sound[CPL_EXPLOSION_SND]->Play(UGKSND_VOLUME_80);
		Explosion.Init(Timer[CPL_RND_PERIOD].GetAlarmPeriodms());
		Explosion.Activate();
		AI_Die();

		//Player loses its life...
		//...Signal the game to change the state
		RTDESK_CMsg *Msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
		SendMsg(Msg, Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);
	}
}

void CPlayer::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	int i, l;

	RenderMode = Mode;
	Explosion.ChangeRenderMode	(Mode);

	for (int r = 0; r < CPL_MAX_REACTORS; r++)
		Reactor[r]->ChangeRenderMode	(Mode);

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		return;
	case CHAR_2D:
		Position.v[YDIM]			= CPL_PLAYER_Y_2D;
		ShootType					= CSH_PLAYER2D;
		ShootSpeed					= CPL_SHOOT2D_SPEED;

		//No laser support when in 2D mode
		for (l = 0; l<CPL_MAX_LASERS; l++){
			Laser[l]->SetDefault();
			Laser[l]->ChangeRenderMode(Mode);
		}

		for (int r = 0; r < CPL_MAX_REACTORS; r++)
			Reactor[r]->SetDefault();

		Rotation.v[XDIM] =	  0.0;
		Rotation.v[YDIM] =    0.0;
		Rotation.v[ZDIM] =    180.0;

		SetScale2D();

		SetAABBInGlobalCoord();
		Passing2D23D = false;
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		Position.v[YDIM]	= CPL_PLAYER_Y_3D;
		ShootType			= CSH_PLAYER3D;
		ShootSpeed			= CPL_SHOOT3D_SPEED;
		
		SetScale3D();

		SetAABBInGlobalCoord(CPL_3D_SIZE_X - Mesh->modelo.rot.z / 250,CPL_3D_SIZE_Y,CPL_3D_SIZE_Z);
		MoveTo(Position);
		
		//Fit Player's mesh into its bounding box
		FitMeshIntoBoundingBox();
		if(Mesh)
			if (Mesh->Loaded)
				Mesh->modelo.scale.y *= 3.15f;

		//Fit Player's laser meshes into their bounding box
		for (i = 0; i < CPL_MAX_LASERS; i++)
		{

			Laser[i]->CharAABB.AABB[CHAR_BBSIZE][XDIM].Value = CharAABB.AABB[CHAR_BBSIZE][XDIM].Value / 3;
			Laser[i]->CharAABB.AABB[CHAR_BBSIZE][YDIM].Value = CharAABB.AABB[CHAR_BBSIZE][YDIM].Value;
#ifdef CHAR_USE_AABB
			Laser[i]->SetAABBInGlobalCoord();
#endif
			Laser[i]->FitMeshIntoBoundingBox();

			Laser[i]->ChangeRenderMode(Mode);
			Laser[i]->Passing2D23D = false;

		}
		ActivateReactor();
		Passing2D23D = false;
		break;
	default: return;
	}

	Speed.v[XDIM]			= 0.0f;	// Player is stopped
	Acceleration.v[XDIM]	= 0.0f;
}

void CPlayer::Render(void)
{
	int l;
	GLboolean	Blending = glIsEnabled(GL_BLEND),
				Alpha	 = glIsEnabled(GL_ALPHA_TEST);

	if (!Alive())
		return;

	glEnable(GL_LIGHTING);			// activate lights on

	//Players render
	glEnable(SIGLB_PLAYER_LIGHT);
	//Disable the navy ships lighting
	glEnable(SIGLB_SHIP_LIGHT);
	glEnable(SIGLB_SHIP2_LIGHT);

	// GAME 2D
	switch (RenderMode)
	{
	case CHAR_2D:
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		
		((CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF])->Textures[IndTexture2D]->SetTexture();

		// ALPHA TEST + BLEND
		if (!Alpha)		glEnable(GL_ALPHA_TEST);								// for TGA alpha test
		if (!Blending)	glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GREATER, 0.3f);								// for TGA alpha test

		Render2D();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		if (!Alpha)  glDisable(GL_ALPHA_TEST);
		if (!Blending) glDisable(GL_BLEND);
		glBlendFunc	(GL_SRC_ALPHA,GL_ONE);
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		glEnable(GL_CULL_FACE);		// Back face culling set on
		glFrontFace(GL_CCW);		// The faces are defined counter clock wise
		glEnable(GL_DEPTH_TEST);	// Occlusion culling set on

		Mesh->modelo.rot.x =  -270.0f;
		Mesh->modelo.pos.x = Position.v[XDIM];

		if (!Passing2D23D)
			Mesh->modelo.pos.y = Position.v[YDIM];

		//Control to rotate Player while moving to left or to right
		Mesh->modelo.rot.z = -Scrolling * 5000;
		if(Mesh->modelo.rot.z > 50) Mesh->modelo.rot.z = 50;
		else if(Mesh->modelo.rot.z < -50) Mesh->modelo.rot.z = -50;  //[-50, 50]

		//Set the scale
		Mesh->modelo.scale.v = Scale.v;

		// Player touched
		if (Hit_duration < CPL_MAX_MAX_HIT_DUR)
		{
			Hit_duration -= 20 * Timer[CPL_RND_PERIOD].GetAlarmPeriodms();
			if (Hit_duration <= 0)
				Hit_duration = CPL_MAX_MAX_HIT_DUR;
		}
		
		// Normal player drawing
		Mesh->modelo.Draw();

		for (unsigned int i = 0; i < CPL_MAX_LASERS; i++)
			Laser[i]->Render();

		//In 2D, no reactor has to be rendered
		for (int r = 0; r < CPL_MAX_REACTORS; r++)
			if (Reactor[r]->Active())
			{
				Reactor[r]->Rotation.Set(Mesh->modelo.rot.x, Mesh->modelo.rot.y, Mesh->modelo.rot.z);
				Reactor[r]->Render();
			}
		break;
	default:;
	}

	//Player Explosions
	if (Explosion.Active())
	{
		Explosion.Render();
		if (!Explosion.Alive() && Lives>0)
			Activate();
	}
}

///Geometric transformation methods
void CPlayer::MoveTo		(float x, float y, float z)	///Moves to the absolute coordinate x,y,z
{
	Vector P;

	P.Set(x, y, z);

	MoveTo(P);
}

void CPlayer::MoveTo		(Vector &Move)				///Moving to an absolute position. It does not matter where the character is

{
	Vector NewPos;

	CCharacter::MoveTo(Move); 

	if (Laser[CPL_LEFT_LASER]->Active()) {
		NewPos = Position;
		NewPos.v[XDIM] += - CharAABB.AABB[CHAR_BBSIZE][XDIM].Value - CPL_LASER_XSHIFT;
		Laser[CPL_LEFT_LASER]->MoveTo(NewPos);
	}

	if (Laser[CPL_RIGHT_LASER]->Active()) {
		NewPos = Position;
		NewPos.v[XDIM] += CharAABB.AABB[CHAR_BBSIZE][XDIM].Value + CPL_LASER_XSHIFT;
		Laser[CPL_RIGHT_LASER]->MoveTo(NewPos);
	}

	for (int r = 0; r < CPL_MAX_REACTORS; r++)
		Reactor[r]->MoveTo(Position);

	//Relative positions of every reactor
	NewPos.Set(-CPL_REACTOR_XSHIFT, -CPL_REACTOR_YSHIFT, CPL_REACTOR_ZSHIFT);
	Reactor[CPL_LEFT_REACTOR]->MoveRelTo(NewPos);
	NewPos.Set(CPL_REACTOR_XSHIFT, -CPL_REACTOR_YSHIFT, CPL_REACTOR_ZSHIFT);
	Reactor[CPL_RIGHT_REACTOR]->MoveRelTo(NewPos);
}

void CPlayer::MoveRelTo	(float x, float y, float z)	///Moves x,y,z units from the to the current position
{
	Vector P;

	P.Set(x, y, z);

	MoveRelTo(P);
}

void CPlayer::MoveRelTo (Vector &Move)	///Moving relative to the character current Position
{
	///Adding relative movement
	Position += Move;
	MoveTo(Position);
}

	//User interaction. One method for every player command sent to the player's avatar
void CPlayer::ManageEvent	(unsigned int Event, bool On)
{
		switch (Event)
		{
		case CPL_MOVE_LEFT:			///The player wants to move to the left
			UserLeft(On);
			break;
		case CPL_MOVE_RIGHT:		///The player wants to move to the right
			UserRight(On);
			break;
		case CPL_SHOOT:				///The player wants to fire a shoot
			if (On) UserShoot();
			break;
		}
}

void CPlayer::UserRight	(bool On)
{
	PositionPrev=Position;
	if (Alive())
		switch(RenderMode)
		{
		case	CHAR_2D:
		case	CHAR_LINES3D:
			State = CPL_MOVING;
			if(On)
				Speed.v[XDIM] = CPL_MAX_SPEED;
			else 
				Speed.v[XDIM] = 0.0f;
			break;
		case	CHAR_3D:
			if (On)
				State = CPL_SPEEDINGUP;
			else
			{
				State = CPL_MOVING;
				Acceleration.v[XDIM] = 0.0f;
			}
		Direction = 1.0f;
		}
}

void CPlayer::UserLeft	(bool On)
{
	PositionPrev=Position;
	if (Alive())
		switch (RenderMode)
		{
		case	CHAR_2D:
		case	CHAR_LINES3D:
			State = CPL_MOVING;
			if (On)
				Speed.v[XDIM] = -CPL_MAX_SPEED;
			else
				Speed.v[XDIM] = 0.0f;
			break;
		case	CHAR_3D:
			if (On)
				State = CPL_SPEEDINGUP;
			else
			{
				State = CPL_MOVING;
				Acceleration.v[XDIM] = 0.0f;
			}
			Direction = -1.0f;
		}
}

void CPlayer::Update(void)
{
	int		l;
	double	msUpdPlayer;
	float Delta;

	#ifdef XM_UPD_TIME_CONT
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
	#endif

	if (Timer[CPL_UPD_PERIOD].IsSounding())
	{
		Timer[CPL_UPD_PERIOD].ResetAlarm();

		//Check for explosions
		if (Explosion.Active())
			Explosion.Update();

		//Check for Laser explosions
		for (l = 0; l<CPL_MAX_LASERS; l++)
			if (Laser[l]->Explosion.Active())
				Laser[l]->Explosion.Update();

#ifndef XM_CONTINUOUS_WITH_SIMULATE_TIME
		LastUpdTime = nowTime;

		msUpdPlayer = abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Speed.v[YDIM]);
		if(msUpdPlayer > SIGLBD_MAX_UPDATETIME) msUpdPlayer = SIGLBD_MAX_UPDATETIME;
		Timer[CPL_UPD_PERIOD].SetAlarm(Timer[CPL_UPD_PERIOD].ms2Ticks(msUpdPlayer));

		//Control to rotate the Player
		if(Scrolling >  0.0001) Scrolling -= 0.001;
		if(Scrolling < -0.0001) Scrolling += 0.001;
#else

		//When moving in 2D, speed is constant. When in 3D, it may vary.
		switch (State)
		{
		case CPL_SPEEDINGUP:
			Acceleration.v[XDIM] = Direction*CPL_MAX_ACCEL;
		case CPL_MOVING:
			Delta = Timer[CPL_UPD_PERIOD].GetAlarmPeriodms();	//Convert to seconds
			switch (RenderMode)
			{
			case	CHAR_2D:
			case	CHAR_LINES3D:
				MoveRelTo(Speed.v[XDIM] * Delta, 0.0f, 0.0f);
				break;
			case	CHAR_3D:
				//viscous friction depending on the speed and position
				if (Position.v[XDIM] < SIGLBD_PG_LEFT + CPL_BOUNDARY_MARGIN || Position.v[XDIM] > SIGLBD_PG_RIGHT - CPL_BOUNDARY_MARGIN)
					//viscous friction increased depending on the position
					Acceleration.v[XDIM] += -Speed.v[XDIM] * CPL_BOUNDARY_FRICTION;
				else 
					Acceleration.v[XDIM] += -Speed.v[XDIM] * CPL_VISCOUS_COEFF;
			
				if (Acceleration.v[XDIM] > CPL_MAX_ACCEL) 
					Acceleration.v[XDIM] = CPL_MAX_ACCEL;
				else if (Acceleration.v[XDIM] < -CPL_MAX_ACCEL)
					Acceleration.v[XDIM] = -CPL_MAX_ACCEL;

				Speed.v[XDIM] += Acceleration.v[XDIM]*Delta;
			}

			//Speed fitting the movement whithin the boundary game limits
			if (Speed.v[XDIM] > CPL_MAX_SPEED)
				Speed.v[XDIM] = CPL_MAX_SPEED;
			if (Speed.v[XDIM] < -CPL_MAX_SPEED)
				Speed.v[XDIM] = -CPL_MAX_SPEED;

			if (Speed.v[XDIM] > -CPL_MIN_SPEED && Speed.v[XDIM] < 0.0f)
			{
				Speed.v[XDIM] = 0.0f;
				Acceleration.v[XDIM] = 0.0f;
			}
			else if (Speed.v[XDIM] < CPL_MIN_SPEED && Speed.v[XDIM] > 0.0f)
			{
				Speed.v[XDIM] = 0.0f;
				Acceleration.v[XDIM] = 0.0f;
			}

			MoveRelTo(Speed.v[XDIM] * Delta * 0.5f, 0.0f, 0.0f);

			//Check for fitting the movement whithin the  boundary game limits
			if (Position.v[XDIM] <= SIGLBD_PG_LEFT)
			{
				Position.v[XDIM]	 = SIGLBD_PG_LEFT;
				Speed.v[XDIM]		 = 0.0f;
				Acceleration.v[XDIM] = 0.0f;
			}

			if (Position.v[XDIM] >= SIGLBD_PG_RIGHT)
			{
				Position.v[XDIM]	 = SIGLBD_PG_RIGHT;
				Speed.v[XDIM]		 = 0.0f;
				Acceleration.v[XDIM] = 0.0f;
			}
			
			//Rotation control of the Player 
			Scrolling = Speed.v[XDIM];

			UpdateCollisionDetection();
			break;
		default:
			break;
		}

		if (Speed.v[XDIM])
		{
			msUpdPlayer = abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value / Speed.v[XDIM]);
			if (msUpdPlayer > SIGLBD_MAX_UPDATETIME) msUpdPlayer = SIGLBD_MAX_UPDATETIME;
		}
		else msUpdPlayer = SIGLBD_MAX_UPDATETIME;

		Timer[CPL_UPD_PERIOD].SetAlarm(Timer[CPL_UPD_PERIOD].ms2Ticks(msUpdPlayer));
#endif
	}

	#ifdef XM_UPD_TIME_CONT
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
	#endif
}

void CPlayer::DiscreteUpdate(void)
{
	double		msUpdPlayer;

	#ifdef XM_UPD_TIME_DISC
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
	#endif

	//Control to rotate the Player
	if(Scrolling >  0.0001) Scrolling -= 0.001;
	if(Scrolling < -0.0001) Scrolling += 0.001;

	msUpdPlayer = abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Speed.v[YDIM]);
	if(msUpdPlayer > SIGLBD_MAX_UPDATETIME) msUpdPlayer = SIGLBD_MAX_UPDATETIME;

	#ifdef XM_UPD_TIME_DISC
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
	#endif

	if(!EndByTime && !EndByFrame){
		#ifdef DEF_RTD_TIME
		TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
		#endif

		SendSelfMsg(msg, Timer[CPL_UPD_PERIOD].ms2Ticks(msUpdPlayer));

		#ifdef DEF_RTD_TIME
		TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
		#endif
	}
}

void CPlayer::ReceiveMessage(RTDESK_CMsg *pMsg)
{
	DiscreteUpdate();
}

void CPlayer::UserShoot	()
{
	static unsigned int shoots = 0;

	Vector	P,	//Shoot initial Position
			S;	//Shoot initial Speed
	CSH_SHOOT_TYPE	LaserShootType;

	if (Alive())
	{
		S.v[XDIM] = S.v[ZDIM] = 0.0f;
		//Generate sound of firing
		if (CHAR_2D == RenderMode)
		{
			Sound[CPL_SHOOT_SND]->Play(UGKSND_LOW_VOLUME);
			
			//Set Y speed
			S.v[YDIM] = CPL_SHOOT2D_SPEED;

			//Set position
			P.Set(Position.v[XDIM], Position.v[YDIM] + .5f, Position.v[ZDIM]);
		}
		else
		{
			Sound[CPL_SHOOT3D_SND]->Play(UGKSND_LOW_VOLUME);

			//Set Y speed
			S.v[YDIM] = CPL_SHOOT3D_SPEED;

			//Set position
			P.Set(Position.v[XDIM], Position.v[YDIM] + 1.5f, Mesh->modelo.pos.z);
		}

		//Generate a new shoot
		//ShootsManager->nextShootVisible = true;
		ShootsManager->NewShoot(ShootType, P, S);

		//Auxiliar shoots with Bonus Weapon
		if (CSH_PLAYER3D_CHEVRON == ShootType)
		{
			P.v[XDIM] += 0.5;
			ShootsManager->NewShoot(ShootType, P, S);
			P.v[XDIM] -= 1.0;	//0.5 to substract the relative movement of +0.5 done before and another -0.5 movement to the other part
			ShootsManager->NewShoot(ShootType, P, S);
			LaserShootType = CSH_AUX_LASER_CHEVRON;
		}
		else LaserShootType = CSH_AUX_LASER;

		//Generate a new shoot for every laser available
		for(unsigned int i = 0; i < CPL_MAX_LASERS;i++)
			if (Laser[i]->Active())
			{
				S.v[XDIM] = S.v[ZDIM] = 0.0f;
				S.v[YDIM] = CPL_SHOOT3D_SPEED;
				ShootsManager->NewShoot(LaserShootType, Laser[i]->Position, S);
			}
	}
}

void CPlayer::SetRenderPeriod(HRT_Timems RP)
{
	unsigned int i;

	Timer[CPL_RND_PERIOD].SetAlarm(RP);
	Timer[CPL_RND_PERIOD].Activate();


	for (int r = 0; r < CPL_MAX_REACTORS; r++)
	{
		Reactor[r]->Timer[CPL_RND_PERIOD].SetAlarm(RP);
		Reactor[r]->Timer[CPL_RND_PERIOD].Activate();
	}

	for (i = 0; i < CPL_MAX_LASERS; i++)
	{
		Laser[i]->Timer[CPL_RND_PERIOD].SetAlarm(RP);
		Laser[i]->Timer[CPL_RND_PERIOD].Activate();
	}
}

void CPlayer::SetUpdatePeriod(HRT_Timems UP)
{
	unsigned int i;

	Timer[CPL_UPD_PERIOD].SetAlarm(UP);
	for (int r = 0; r < CPL_MAX_REACTORS; r++)
		Reactor[r]->Timer[CPL_UPD_PERIOD].SetAlarm(UP);

	for (i = 0; i < CPL_MAX_LASERS; i++)
		Laser[i]->Timer[CPL_UPD_PERIOD].SetAlarm(UP);
}
