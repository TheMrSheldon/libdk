
class DKWSInterface {
	playerhands = ["south", "west", "north", "east"];

	constructor(endpoint) {
		this.callbacks = {'stateupdate': this.onStateUpdate.bind(this), 'place': this.onPlacement.bind(this)}

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
			var element = document.getElementById(`fan_${this.playerhands[i]}`);
			element.querySelectorAll('*').forEach(n => n.remove());
			
			packet.hands[i].forEach(card_value => {
				var card = new HTMLCard();
				card.dataset['value'] = card_value;
				element.appendChild(card);
			});
			console.log(element);

		}
	}

	onPlacement(packet) {
		var hand = document.getElementById(`fan_${this.playerhands[packet.player]}`);
		var center = document.getElementById(`center_${this.playerhands[packet.player]}`);
		center.dataset['value'] = "SLOT";
		hand.querySelector(`[data-value="${packet.card}"]`).moveTo(center);
	}
}