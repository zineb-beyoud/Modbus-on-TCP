#include <stdio.h>
#include "MBMESN_TCP.h"
#include "MBMESN_errorstatus.h"
#include <winsock2.h>
#define Max_clients 30

    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort=502;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */
    static int32_t Id = 0;
    static int counter = 0;
    WSADATA wsaData;                 /* Structure for WinSock setup communication */


/** Démarre le serveur Modbus on TCP sur le port bien connu 502 et commence à écouter les demandes de connexion.*/

MBMESN_errorstatus_t MBMESN_StartTcpServer(){

    WSAStartup(MAKEWORD(2, 0), &wsaData);
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

     /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));
    listen(servSock, MAXPENDING);

    return MBMESN_OK;
    }




/** Traite ou attend une demande de connexion et, lorsqu'une demande est acceptée, attribue un identifiant au
client sous forme de nombre entier et le copie dans la variable « clientId ».*/

MBMESN_errorstatus_t MBMESN_GetClient(int32_t * clientId){
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) DieWithError("accept() failed");
        else{
            *(clientId+counter) = Id++;
			counter++;
			}
        /* clntSock is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        return MBMESN_OK;
    }


/** Ferme une connexion avec un client après que le client a demandé la déconnexion ou suite à une erreur ou si le nombre
 de connexion maximum est atteint (ce dernier cas ne s'applique pas pour ces TP car vous ne traitez qu'un seul client à la fois).*/

MBMESN_errorstatus_t MBMESN_CloseConnectionWithClient(int32_t clientId){

    if (MBMESN_GetClient(cliendId)!=MBMESN_OK || counter> Max_clients){
        closesocket(clntSock);
        return MBMESN_OK;
    }
    else
        return MBMESN_ERROR;
}

/**Envoie le message contenu dans le tableau pointé par dataToSend, de longueur lengthOfData, au client identifié par clientId.*/

MBMESN_errorstatus_t MBMESN_tcpWriteToClient(int32_t clientId, uint8_t* dataToSend,uint32_t lengthOfData){

    if (MBMESN_GetClient(cliendId)==MBMESN_OK){
        send(clntSock, dataToSend, lengthOfData, 0);
        return MBMESN_OK;
    }
    else
        return MBMESN_ERROR;

}

/**Attend un message de la part du client identifié par clientId qui est déjà connecté au serveur,
 data indique le tableau à utiliser pour stocker le message reçu,lengthOfData la taille de ce tableau.
 La quantité de données effectivement reçues est copiée dans l'entier pointé par nbBytesRead.*/
MBMESN_errorstatus_t MBMESN_tcpReadFromClient(int32_t clientId, uint8_t * data, uint16_t lengthOfData, uint32_t * nbBytesRead){

 if (MBMESN_GetClient(cliendId)==MBMESN_OK){
    recvMsgSize = recv(clntSock, data, lengthOfData, 0);
    *nbBytesRead = *(data + 4) >> 8 | *(data + 5);
 }
else
    return MBMESN_ERROR;




}
