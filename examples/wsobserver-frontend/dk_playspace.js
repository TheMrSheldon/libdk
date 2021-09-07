
var board = {
	directions: ["south", "west", "north", "east"],
	localPlayer: 0,
	turn: 0,
	roundEnded: true,
	getDirection(playerIdx) {
		const idx = (playerIdx+4-this.localPlayer)%4
		return this.directions[idx]
	},
	getPlayerDiv(playerIdx) {
		return document.getElementById(`player_${this.getDirection(playerIdx)}`)
	},
	getHand(playerIdx) {
		return document.getElementById(`fan_${this.getDirection(playerIdx)}`)
	},
	getCenter(playerIdx) {
		return document.getElementById(`center_${this.getDirection(playerIdx)}`)
	},
	setPlayerName(index, name) {
		this.getPlayerDiv(index).querySelector('.playername').innerHTML = name;
	},
	setLocalPlayer(index) {
		this.localPlayer = index
	},
	setHand(playerIdx, hand, callback = null) {
		const fan = this.getHand(playerIdx)
		fan.querySelectorAll('*').forEach(n => n.remove());
		
		hand.forEach(card_value => {
			var card = new HTMLCard();
			card.dataset['value'] = card_value;
			if (callback != null) {
				card.className = 'clickable';
				card.onclick = () => {callback(playerIdx, card_value);};
			}
			fan.appendChild(card);
		});
	},
	placeCard(playerIdx, cardStr) {
		if (this.roundEnded)
			this.resetTableToNewRound()
		var hand = this.getHand(playerIdx)
		var center = this.getCenter(playerIdx)
		center.dataset['value'] = "SLOT"
		var card = hand.querySelector(`[data-value="${cardStr}"], [data-value="HIDDEN"]`)
		card.dataset['value'] = cardStr
		center.style.zIndex = this.turn
		card.moveTo(center)
		this.turn = this.turn+1
	},
	setRoundEnded() {
		this.roundEnded = true
	},
	resetTableToNewRound() {
		this.roundEnded = false
		this.turn = 0 //TODO: set to winner of prev round
		for (i = 0; i < 4; i++) {
			var center = this.getCenter(i)
			center.dataset['value'] = "INVISIBLE"
		}
	}
};
