
class DKWSInterface {

	constructor(endpoint) {
		this.callbacks = {'stateupdate': this.onStateUpdate.bind(this), 'place': this.onPlacement.bind(this), 'roundend': this.onRoundEndMsg.bind(this)}

		this.socket = new WebSocket(endpoint);
		this.socket.binaryType = "arraybuffer";
		this.socket.onopen = (event) => { console.log("connection opened"); }
		this.socket.onclose = (event) => { console.log("connection closed"); }
		this.socket.onerror = (error) => { console.log("connection error "+error.message); }
		this.socket.onmessage = (event) => {
			var obj = JSON.parse(event.data);
			this.callbacks[obj.type](obj);
		}
	}

	onStateUpdate(packet) {
		for (i = 0; i < 4; i++) {
			board.setHand(i, packet.hands[i], false)
		}
	}

	onPlacement(packet) {
		board.placeCard(packet.player, packet.card)
	}
	
	onRoundEndMsg(packet) {
		board.setRoundEnded()
	}
}