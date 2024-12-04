const phoneNumberInput = document.getElementById('phoneNumber');
const responsibleNameInput = document.getElementById('responsibleName');
const addPhoneButton = document.getElementById('addPhone');
const phoneList = document.getElementById('phoneList');

// Atualiza a lista ao carregar a página
document.addEventListener('DOMContentLoaded', updatePhoneList);

// Função para formatar o número de telefone
function formatPhoneNumber(value) {
    if (!value) return value; // Retorna vazio se o valor estiver vazio
    const phoneNumber = value.replace(/[^\d]/g, ''); // Remove caracteres não numéricos
    const phoneNumberLength = phoneNumber.length;

    // Define os formatos conforme o tamanho do número
    if (phoneNumberLength < 3) return `+${phoneNumber}`;
    if (phoneNumberLength < 5) return `+${phoneNumber.slice(0, 2)} (${phoneNumber.slice(2)})`;
    if (phoneNumberLength < 10) return `+${phoneNumber.slice(0, 2)} (${phoneNumber.slice(2, 4)}) ${phoneNumber.slice(4)}`;
    return `+${phoneNumber.slice(0, 2)} (${phoneNumber.slice(2, 4)}) ${phoneNumber.slice(4, 9)}-${phoneNumber.slice(9, 13)}`;
}

// Evento para aplicar a formatação enquanto o usuário digita
phoneNumberInput.addEventListener('input', (e) => {
    const formattedInputValue = formatPhoneNumber(e.target.value);
    e.target.value = formattedInputValue; // Atualiza o valor formatado no campo
});

// Adiciona um número no servidor e atualiza a lista
addPhoneButton.addEventListener('click', () => {
    const phoneNumber = phoneNumberInput.value.trim();
    const responsibleName = responsibleNameInput.value.trim();

    if (phoneNumber && responsibleName) {
        addNumber(phoneNumber, responsibleName); // Salva no servidor
        phoneNumberInput.value = '';
        responsibleNameInput.value = '';
    } else {
        alert('Por favor, preencha o nome e o número de telefone.');
    }
});

// Salva o número no servidor usando a rota /add_number
function addNumber(phoneNumber, responsibleName) {
    fetch('/add_number', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ phoneNumber, responsibleName }),
    })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                alert(data.message);
                updatePhoneList(); // Atualiza lista após salvar
            } else {
                alert('Erro: ' + data.error);
            }
        })
        .catch(error => {
            console.error('Erro:', error);
        });
}

// Busca os números salvos no servidor e atualiza a área da lista
function updatePhoneList() {
    fetch('/list_numbers', {
        method: 'GET',
    })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                renderPhoneList(data.numbers); // Renderiza os números retornados
            } else {
                console.error('Erro ao buscar números cadastrados:', data.error);
            }
        })
        .catch(error => {
            console.error('Erro:', error);
        });
}

// Renderiza a lista de números na interface
function renderPhoneList(phoneNumbers) {
    phoneList.innerHTML = ''; // Limpa a lista antes de renderizar
    phoneNumbers.forEach((entry, index) => {
        const li = document.createElement('li');
        li.className = 'phone-item';
        li.innerHTML = `
            <label>
                <span>${entry.responsibleName}: ${entry.phoneNumber}</span>
            </label>
            <button class="remove-btn" data-phone="${entry.phoneNumber}">Remover</button>
        `;
        phoneList.appendChild(li);
    });
}

// Remove um número quando o botão "Remover" é clicado
phoneList.addEventListener('click', (e) => {
    if (e.target.classList.contains('remove-btn')) {
        const phoneNumber = e.target.getAttribute('data-phone'); // Obtem o número do atributo
        deleteNumber(phoneNumber)
            .then(() => {
                updatePhoneList(); // Atualiza a lista após a exclusão
            })
            .catch(error => {
                console.error('Erro ao remover número:', error);
                alert('Não foi possível remover o número. Tente novamente.');
            });
    }
});

// Função para remover número no servidor
function deleteNumber(phoneNumber) {
    return fetch('/delete_number', {
        method: 'DELETE',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ phoneNumber }),
    }).then(response => {
        if (!response.ok) {
            return response.json().then(err => {
                throw new Error(err.error || 'Erro desconhecido ao excluir número.');
            });
        }
        return response.json();
    });
}
