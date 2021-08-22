class HTMLFan extends HTMLElement {
	static #DegToRad = Math.PI/180;
	static #CardAspect = 88/63; //Height per Width
	radius = 680//340;
	cardWidth = 126//63;
	observer = new MutationObserver(HTMLFan.#onChangeObserved);

	constructor() {
		super();
		this.observer.observe(this, { attributes: false, childList: true, subtree: false });
	}

	update() {
		var cards = this.getElementsByTagName("x-card");
		const degPerCard = 3;
		const firstCardAngle = (0-cards.length*degPerCard)/2;
		const firstCardOffsetX = Math.sin(firstCardAngle*HTMLFan.#DegToRad)*this.radius
		const firstCardOffsetY = Math.cos(firstCardAngle*HTMLFan.#DegToRad)*this.radius
		var totWidth = 0;
		var height = 0;
		for (var i = 0; i < cards.length; ++i) {
			var child = cards[i];
			const angle = firstCardAngle + i*degPerCard;
			const xoffset = Math.sin(angle*HTMLFan.#DegToRad)*this.radius - firstCardOffsetX;
			const yoffset = -(Math.cos(angle*HTMLFan.#DegToRad)*this.radius - firstCardOffsetY);
			child.style.transform = `rotate(${angle}deg)`;
			child.style.left = xoffset;
			child.style.top = yoffset;

			child.onmouseover = (e) => {
				if (e.target.classList.contains("clickable")) {
					e.target.style.left = xoffset+Math.sin(angle*HTMLFan.#DegToRad)*100
					e.target.style.top = yoffset-Math.cos(angle*HTMLFan.#DegToRad)*100
				}
			};
			child.onmouseout = (e) => {
				e.target.style.left = xoffset;
				e.target.style.top = yoffset;
			}
		}

		this.style.width = `${cards.length * this.cardWidth/4}pt`;
		this.style.height = `${this.cardWidth * HTMLFan.#CardAspect}pt`;
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