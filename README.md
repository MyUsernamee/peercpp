
This is mean to be a cpp version of peerjs, not an exact copy, but the same easy opf use and all that

https://github.com/ambianic/peerjs-python/blob/master/src/peerjs/dataconnection.py

__Used the above as reference__

- Two parts
    - Signaling
        - Websockets to PeerJs Server
            - Connect to https://0.peerjs.com/peerjs/id to obtain id
            - Open a websocket to https://0.peerjs.com/?id=(id_here)&key=peerjs
            - 
    - Peer negotiation
    - Peer connection