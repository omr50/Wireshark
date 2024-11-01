const { app, BrowserWindow, Menu } = require('electron');
const path = require('path')

const isDev = true; 

function createMainWindow() {
    const mainWindow = new BrowserWindow({
        'title': "Packet Analyzer",
        'width': (!isDev) ? 500 : 1000,
        'height': 800
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
        'height': 600
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



