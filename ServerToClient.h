#ifndef __ServertoClient_h_
#define __ServertoClient_h_

#include <btBulletDynamicsCommon.h>

class ServerToClient{
	protected:
		btTransform clientBallTrans;
		btTransform serverBallTrans;
		float targPose[7];
		float padPose[7];
		int score;
		int sound;
	public:
		ServerToClient();
		~ServerToClient();
		void setSound(int&);
		void setClientBall(btTransform&);
		void setServerBall(btTransform&);
		void setTarget(float*);
		void setPaddle(float*);
		void setScore(int&);
		
        btTransform& getClientBall();
		btTransform& getServerBall();
		float* getTarget();
		float* getServerPaddle();
		int& getScore();
		int& getSound();
};

ServerToClient::ServerToClient(){}
ServerToClient::~ServerToClient(){}

void ServerToClient::setSound(int& s){
    sound = s;
}

void ServerToClient::setClientBall(btTransform& trans){
    clientBallTrans = trans;
}

void ServerToClient::setServerBall(btTransform& trans){
    serverBallTrans = trans;
}

void ServerToClient::setTarget(float* pose){
    for(int i = 0; i < 7; i++){
        targPose[i] = pose[i];
    }
}

void ServerToClient::setPaddle(float* pose){
    for(int i = 0; i < 7; i++){
        padPose[i] = pose[i];
    }
}

void ServerToClient::setScore(int& n){
    score = n;
}

btTransform& ServerToClient::getClientBall(){
    return clientBallTrans;
}

btTransform& ServerToClient::getServerBall(){
    return serverBallTrans;
}

float* ServerToClient::getTarget(){
    return targPose;
}

float* ServerToClient::getServerPaddle(){
    return padPose;
}

int& ServerToClient::getScore(){
    return score;
}

int& ServerToClient::getSound(){
    return sound;
}



#endif
