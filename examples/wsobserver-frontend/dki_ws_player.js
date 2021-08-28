
class DKWSInterface {
	playerhands = ["south", "west", "north", "east"];
	myindex = -1;

	constructor(endpoint) {
		this.callbacks = {'stateupdate': this.onStateUpdateMsg.bind(this), 'place': this.onPlacementMsg.bind(this), 'getaction': this.onGetActionMsg.bind(this)}

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
		this.myindex = packet.position;
		var myfan = document.getElementById(`fan_${this.playerhands[this.myindex]}`);
		myfan.querySelectorAll('*').forEach(n => n.remove());
		
		packet.hand.forEach(card_value => {
			var card = new HTMLCard();
			card.dataset['value'] = card_value;
			card.className = 'clickable';
			card.onclick = () => {this.placeCard(card_value);};
			myfan.appendChild(card);
		});
		myfan.style.opacity = .5;
	}

	onPlacementMsg(packet) {
		var hand = document.getElementById(`fan_${this.playerhands[packet.player]}`);
		var center = document.getElementById(`center_${this.playerhands[packet.player]}`);
		center.dataset['value'] = "SLOT";
		var card = hand.querySelector(`[data-value="${packet.card}"], [data-value="HIDDEN"]`);
		card.dataset['value'] = packet.card;
		card.moveTo(center);
	}

	onGetActionMsg(packet) {
		var hand = document.getElementById(`fan_${this.playerhands[this.myindex]}`);
		hand.style.opacity = 1;
	}

	placeCard(card) {
		var packet = {type: 'place', card: card};
		this.socket.send(JSON.stringify(packet));
		
		var hand = document.getElementById(`fan_${this.playerhands[this.myindex]}`);
		hand.style.opacity = .5;
	}

	announcement() {
		//TODO: implement
	}
}