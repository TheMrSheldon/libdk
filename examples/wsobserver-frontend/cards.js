class HTMLFan extends HTMLElement {
	static #DegToRad = Math.PI/180;
	static #CardAspect = 88/63; //Height per Width
	radius = 680//340;
	cardWidth = 126//63;
	observer = new MutationObserver(HTMLFan.#onChangeObserved);

	constructor() {
		super();
		this.observer.observe(this, { attributes: true, childList: true, subtree: false });
	}

	update() {
		var cards = this.getElementsByTagName("x-card");
		var dim = HTMLFan.#calcFan(Number(this.dataset["angle"]), 2, cards.length, this.radius, this.cardWidth, this.cardWidth * HTMLFan.#CardAspect);
		for (var i = 0; i < cards.length; ++i) {
			const card = cards[i];
			const angle = dim.cards[i].angle
			card.style.transform = `translate(${-this.cardWidth/2-dim.min.x}pt, ${-this.cardHeight()/2+dim.max.y}pt) rotate(${-angle+90}deg) translateY(${-this.radius}pt)`
			
			card.onmouseover = (e) => {
				if (e.target.classList.contains("clickable")) {
					card.style.transform = `translate(${-this.cardWidth/2-dim.min.x}pt, ${-this.cardHeight()/2+dim.max.y}pt) rotate(${-angle+90}deg) translateY(${-this.radius-100}pt)`
				}
			};
			card.onmouseout = (e) => {
				card.style.transform = `translate(${-this.cardWidth/2-dim.min.x}pt, ${-this.cardHeight()/2+dim.max.y}pt) rotate(${-angle+90}deg) translateY(${-this.radius}pt)`
			}
		}
		this.style.width = `${dim.width}pt`;
		this.style.height = `${dim.height}pt`;
	}

	cardHeight() {
		return this.cardWidth * HTMLFan.#CardAspect
	}

	static #calcFan(baseAngle, degPerCard, numCards, radius, cardWidth, cardHeight) {
		var cards = [];
		const firstCardAngle = baseAngle+(numCards-1)*degPerCard/2 + 90;
		var min = {x:  99999, y:  99999};
		var max = {x: -99999, y: -99999};
		for (var i = 0; i < numCards; ++i) {
			const angle = firstCardAngle - i*degPerCard;
			const angRad = angle*HTMLFan.#DegToRad;
			const xoffset = Math.cos(angRad)*radius
			const yoffset = Math.sin(angRad)*radius
			// Calculate corner-coords of the card
			const hEdge = {x: Math.cos(angRad-Math.PI/2)*cardWidth, y: Math.sin(angRad-Math.PI/2)*cardWidth};
			const vEdge = {x: Math.cos(angRad)*cardHeight, y: Math.sin(angRad)*cardHeight};
			const bl = {x: xoffset-hEdge.x/2-vEdge.x/2, y: yoffset-hEdge.y/2-vEdge.y/2}
			const br = {x: bl.x+hEdge.x, y: bl.y+hEdge.y}
			const tl = {x: bl.x+vEdge.x, y: bl.y+vEdge.y}
			const tr = {x: tl.x+hEdge.x, y: tl.y+hEdge.y}
			// Store the card's parameters
			min.x = Math.min(min.x, bl.x, br.x, tl.x, tr.x)
			min.y = Math.min(min.y, bl.y, br.y, tl.y, tr.y)
			max.x = Math.max(max.x, bl.x, br.x, tl.x, tr.x)
			max.y = Math.max(max.y, bl.y, br.y, tl.y, tr.y)
			cards.push({angle: angle, xoffset: xoffset, yoffset: yoffset});
		}
		return {width: max.x-min.x, height: max.y-min.y, min: min, max: max, cards: cards};
	}

	static #onChangeObserved(mutationsList, observer) {
		for(const mutation of mutationsList) {
			if (mutation.type === 'childList') {
				mutation.target.update();
			} else if (mutation.type === 'attributes') { /*attribute was changed*/ }
		}
	}
}

class HTMLCard extends HTMLElement {
	constructor() {
		super();

	}

	moveTo(fanOrSlot) {
		if (fanOrSlot instanceof HTMLFan) {
			fanOrSlot.appendChild(this)
		} else if (fanOrSlot instanceof HTMLCard && (fanOrSlot.dataset['value'] == 'SLOT' || fanOrSlot.dataset['value'] == 'INVISIBLE')) {
			//fanOrSlot.parentNode.replaceChild(this, fanOrSlot);
			fanOrSlot.dataset['value'] = this.dataset['value']
			this.parentNode.removeChild(this)
		} else {
			console.error("Invalid move target");
		}
	}
}

customElements.define("x-fan", HTMLFan)
customElements.define("x-card", HTMLCard)