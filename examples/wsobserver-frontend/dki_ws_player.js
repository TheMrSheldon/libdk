
class DKWSInterface {

	constructor(endpoint) {
		this.callbacks = {
			'stateupdate': this.onStateUpdateMsg.bind(this), 'place': this.onPlacementMsg.bind(this), 'getaction': this.onGetActionMsg.bind(this), 'roundend': this.onRoundEndMsg.bind(this)
		}

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

	onStateUpdateMsg(packet) {
		board.setLocalPlayer(packet.position)
		board.setHand(packet.position, packet.hand, (playerIdx, cardStr) => {this.placeCard(cardStr)})
		board.getHand(board.localPlayer).style.opacity = .5
		for (var i = 0; i < 4; ++i) {
			board.setPlayerName(i, `Player ${i}`)
		}
	}

	onPlacementMsg(packet) {
		board.placeCard(packet.player, packet.card)
		board.getHand(board.localPlayer).style.opacity = .5
	}

	onGetActionMsg(packet) {
		board.getHand(board.localPlayer).style.opacity = 1;
	}

	onRoundEndMsg(packet) {
		board.setRoundEnded()
	}

	placeCard(cardStr) {
		var packet = {type: 'place', card: cardStr};
		this.socket.send(JSON.stringify(packet));
	}

	announcement() {
		//TODO: implement
	}
}