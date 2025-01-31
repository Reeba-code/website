const titleText = "Welcome to the MDiHub Cisco EDGE Center";
const titleElement = document.getElementById("fade-in-title");
const words = titleText.split(" ");

let currentWordIndex = 0;

function showNextWord() {
    if (currentWordIndex < words.length) {
        const wordSpan = document.createElement("span");
        wordSpan.textContent = words[currentWordIndex] + " ";
        titleElement.appendChild(wordSpan);
        
      
        setTimeout(() => {
            wordSpan.classList.add("show");
        }, 10);

        currentWordIndex++;
        
        setTimeout(showNextWord, 500); 
    }
}

window.onload = showNextWord;

document.addEventListener('DOMContentLoaded', function () {
  
  const content = document.querySelector('.content');
  content.style.display = 'none';

 
  setTimeout(() => {
    
      const loader = document.querySelector('.loading');
      loader.style.display = 'none';
      
     
      content.style.display = 'block';

      
      window.location.href = 'NMM.html';
  }, 5000); 
});

function toggleMenu() {
    const nav = document.getElementById('mobile-nav');
    nav.classList.toggle('mobile-nav-open');
}

