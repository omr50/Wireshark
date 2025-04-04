const { app, BrowserWindow, Menu } = require('electron');
const path = require('path')
const { createTCP_Server } = require('./TLS_Server');

const isDev = true; 

let mainWindow;
function createMainWindow() {
    mainWindow = new BrowserWindow({
        'title': "Packet Analyzer",
        'width': (!isDev) ? 700 : 1200,
        'height': 1000,
        'minWidth': 600,
        'minHeight': 600,
        'webPreferences': {
            nodeIntegration: true,
            contextIsolation: false,
        }
    })

    mainWindow.loadFile(path.join(__dirname, './renderer/index.html'));
    if (isDev) {
        console.log("opened dev tools")
        mainWindow.webContents.openDevTools();
    }
}


function createAboutWindow() {
    const aboutWindow = new BrowserWindow({
        'title': "About",
        'width': (!isDev) ? 300 : 800,
        'height': 600,
    })

    aboutWindow.loadFile(path.join(__dirname, './renderer/about.html'));
    if (isDev) {
        aboutWindow.webContents.openDevTools();
    }
}

const menu = [
    {
        label: 'File',
        submenu: [
            {
                label: 'Save',
                click: () => {console.log("pretend save for now!")},
                accelerator: 'CmdOrCtrl+S'
            }
        ]
    },
    {
        label: 'Window',
        submenu: [
            {
                label: 'Quit',
                click: () => app.quit(),
                accelerator: 'CmdOrCtrl+W'
            }
        ]
    },
    {
        label: 'Help',
        submenu: [
            {
                label: 'About',
                click: () => createAboutWindow(),
                accelerator: 'CmdOrCtrl+W'
            }
        ]
    }
]


app.whenReady().then(() => {
    createMainWindow();
    createTCP_Server(mainWindow, 8000);
    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createMainWindow();
        }
    })
})


const mainMenu = Menu.buildFromTemplate(menu);
Menu.setApplicationMenu(mainMenu);

app.on('window-all-closed', () => {
    app.quit();
})



