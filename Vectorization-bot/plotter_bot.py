import logging
import numpy as np
import cairosvg as svg
from random import *
from linedraw import *
from io import BytesIO
from typing import Final
from telegram import Update, File, InlineKeyboardButton, InlineKeyboardMarkup
from telegram.ext import ContextTypes, Application, CommandHandler, MessageHandler, filters, ConversationHandler, CallbackQueryHandler


Username: Final = '@pen_plotter_bot'
Token: Final = '6617301227:AAGX9f5d5BvC66KXAy0_EKVXuuOltDIiJtQ' 
Ohearn = ['https://media.tenor.com/IX3oamddJ9cAAAAC/starwell-baby-don%27t-hurt-me.gif',
          'https://media.tenor.com/GUhnxCpzr78AAAAC/mike-ohearn-baby-dont-hurt-me.gif',
          'https://media.tenor.com/gfz3LnymsS4AAAAd/baby-dont-hurt-me-meme.gif',
          'https://media.tenor.com/aF9g0BotesYAAAAM/mike-o-heam-sigma-male.gif']

WAIT, UPLOAD, CONFIRMATION = range(3)

logging.basicConfig(
    format = '%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level = logging.INFO
)

# Entry point: Starts conversation by simply greeting the user -> WAIT state
async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = f'Welcome {update.effective_user.full_name}!\nChoose a command!'
    )
    
    return WAIT

# Entry point: Restarts conversation from the start after bot shutdown -> WAIT state
async def restart(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Welcome back! The bot had shutdown but is now able to function again!' 
    )
    
    return WAIT

# Fallback (to change): Cancels the conversation -> WAIT state
async def cancel(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Canceling the operation..'
    )
    
    return WAIT

# WAIT component: what is love -> WAIT state
async def mike(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.sendDocument(
        chat_id = update.message.chat_id,
        document = Ohearn[randint(0, len(Ohearn) - 1)]
    )
    
    return WAIT

# WAIT component: Communicates that the system now waits for a picture -> UPLOAD state
async def upload(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Upload a picture you wish to have vectorized!'
    )
    
    return UPLOAD

# UPLOAD component: Receives unexpected message (not a picture) and awaits for an image -> UPLOAD state
async def reject(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Message is NOT a picture, awaiting for a new picture!'
    )
    
    return UPLOAD

# UPLOAD component - Core component: Takes care of all the processing -> CONFIRMATION state
async def process(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Thank you! I will now start the image processing'
    )
    
    # Gets uploaded image in the third best resolution (hardware optimization reasons)
    file = await context.bot.get_file(update.message.photo[-1].file_id)
    # Saves in primary memory the image data
    obj_file = await file.download_as_bytearray()
    
    # Decodes image data in OpenCV format [numpy array with 0 - 255 (8 bits) format channels]
    image = cv2.imdecode(np.frombuffer(BytesIO(obj_file).read(), np.uint8), 1)
    # Saves image for review and troubleshooting, but also for vectorization, in PNG format
    cv2.imwrite('images/photo.png', image)
    
    # Prepares data for image scaling
    size = image.shape[0:2]
    
    # Frees memory
    file = None
    obj_file = None
    image = None
    
    # If image decoding went well up until this point, gives feedback
    await update.message.reply_text('Image received. Please hold tight for feedback while I process the image.')
    
    # Image Processing
    await image_to_json('photo', draw_contours = 2, draw_hatch = 16, message = update.message, resolution=min(size), size = size, bounds = (5.0, -2.5, 15.0, 7.5))
    
    # Saves svg vectorized image as a png for user feedback 
    svg.svg2png(url = 'images/photo.svg', write_to = 'images/vectors.png')

    # Sends the vectorized image to the user to confirm the drawing
    await update.message.reply_photo(photo = open('images/vectors.png', 'rb'))
    await update.message.reply_text(
        text = 'The picture has been fully processed. Do you wish to continue with the operation and forward the data?', 
        reply_markup = InlineKeyboardMarkup([[InlineKeyboardButton('Yes', callback_data = 'ul_confirmed'), InlineKeyboardButton('No', callback_data = 'ul_denied')]])
    )
    
    return CONFIRMATION

# CONFIRMATION component: Takes care of conversation flow once image is vectorized -> Dynamically asserted state
async def confirmation(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    query = update.callback_query
    await query.answer()
    nextState = WAIT
    
    # Python 3.10> structure (similar to C switch/case)
    match query.data:
        # Work in Progress - User has accepted the image which is gonna be sent to the microcontroller for drawing -> TBD
        case 'ul_confirmed':
            await update.callback_query.message.reply_text(
                text = 'The image has been sent to be drawn.\nThank you.'
            )
            import os
            os.system('python3 ../rasp-communication/communication_rasp.py')
        # User refused the image so they're prompted to choose whether they want another image or to cancel the operation -> CONFIRMATION state
        case 'ul_denied':
            await update.callback_query.message.reply_text(
                text = 'Do you wish to upload a new picture or cancel the operation?',
                reply_markup = InlineKeyboardMarkup([[InlineKeyboardButton('New picture', callback_data = 'new_pic'), InlineKeyboardButton('Cancel operation', callback_data = 'cancel_op')]])
            )
            nextState = CONFIRMATION
        # User chooses to upload a new picture -> UPLOAD state
        case 'new_pic':
            await update.callback_query.message.reply_text(
                text = 'Upload a picture you wish to have vectorized!'
            )
            nextState = UPLOAD
        # User chooses to cancel the operation -> WAIT state
        case 'cancel_op':
            await update.callback_query.message.reply_text(
                text = 'Canceling the operation..'
            )
        # Shouldn't occur, but if none of the confirmation transactions happen the system goes back to square one 
        case _:
            await update.callback_query.message.reply_text(
                text = 'There has been an issue with your request.'
            )
    await query.delete_message()
    return nextState

if __name__ == "__main__":
    # Configures application
    app = Application.builder().token(Token).build()

    # Conversation Handler for application (bot)
    app.add_handler(ConversationHandler(       
        # Entry points for conversation
        # - /start command
        # - Any message after bot has shutdown         
        entry_points = [CommandHandler('start', start), MessageHandler(filters.ALL, restart)],
        # Conversation states, handle message transactions
        # - WAIT: awaits for user to wish to upload the image to vectorize
        # - UPLOAD: waits for picture to vectorize, rejects anything else
        # - CONFIRMATION: handles all user confirmation (picture is correct, vectorized image is to their liking, they want to start the drawing process)
        states = {
            WAIT: [
                MessageHandler(~filters.COMMAND, mike),
                CommandHandler('upload', upload)
            ],
            UPLOAD: [
                MessageHandler(~filters.PHOTO & (~filters.COMMAND), reject),
                MessageHandler(filters.PHOTO, process)
            ],
            CONFIRMATION: [
                CallbackQueryHandler(confirmation)
            ]
        },
        # Conversation fallbacks, conditions that make the conversation stop TODO: fix
        fallbacks = [CommandHandler('cancel', cancel)]
    ))
    
    # Starts application
    app.run_polling()
