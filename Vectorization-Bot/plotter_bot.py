#TODO: add connective functionalities for transmission of vector data
#      dynamic command utilities
#      structuralized confirmation for image vectorization
#      feedback utilities
#      overall concise subroutine with user fliendy interfacement
#      more to come..

import logging
import numpy as np
import aspose.words as aw
from random import *
from linedraw import *
from io import BytesIO
from typing import Final
from telegram import Update, File
from telegram.ext import ContextTypes, Application, CommandHandler, MessageHandler, filters, ConversationHandler


Username: Final = '@pen_plotter_bot'
Token: Final = 'TOKEN'
Ohearn = ['https://media.tenor.com/IX3oamddJ9cAAAAC/starwell-baby-don%27t-hurt-me.gif',
          'https://media.tenor.com/GUhnxCpzr78AAAAC/mike-ohearn-baby-dont-hurt-me.gif',
          'https://media.tenor.com/gfz3LnymsS4AAAAd/baby-dont-hurt-me-meme.gif',
          'https://media.tenor.com/aF9g0BotesYAAAAM/mike-o-heam-sigma-male.gif']

WAIT, UPLOAD = range(2)

logging.basicConfig(
    format = '%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level = logging.INFO
)

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = f'Welcome {update.effective_user.full_name}!\nChoose a command!'
    )
    
    return WAIT

async def cancel(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Canceling the operation..'
    )
    
    return WAIT

async def mike(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.sendDocument(
        chat_id = update.message.chat_id,
        document = Ohearn[randint(0, len(Ohearn) - 1)]
    )
    
    return WAIT

async def upload(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Upload a picture you wish to have vectorized!'
    )
    
    return UPLOAD

async def reject(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Message is NOT a picture, awaiting for a new picture!'
    )
    
    return UPLOAD

async def process(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Thank you! I will now start the image processing'
    )
    
    file = await context.bot.get_file(update.message.photo[-1].file_id)
    obj_file = await file.download_as_bytearray()
    
    image = cv2.imdecode(np.frombuffer(BytesIO(obj_file).read(), np.uint8), 1)
    cv2.imwrite('images/photo.png', image)
    cv2.imshow('Image', image)
    cv2.waitKey(5000)
    cv2.destroyAllWindows()
    
    file = None
    obj_file = None
    image = None
    
    await update.message.reply_text('Image received. Please hold tight for feedback while I lazily process the image.')
    
    image_to_json('photo', draw_contours = 2, draw_hatch = 16)
    
    img = aw.DocumentBuilder(aw.Document()).insert_image('images/photo.svg')
    img.image_data.save('images/vectors.png')
    
    await update.message.reply_photo(photo = open('images/vectors.png', 'rb'))
    await update.message.reply_text('The picture has been fully processed. \nThis is currently a demo: more to come soon')
    
    return WAIT

if __name__ == "__main__":
    app = Application.builder().token(Token).build()
    
    app.add_handler(ConversationHandler(
        entry_points = [CommandHandler('start', start)],
        states = {
            WAIT: [
                MessageHandler(filters.TEXT & (~filters.COMMAND), mike),
                CommandHandler('upload', upload)
            ],
            UPLOAD: [
                MessageHandler(~filters.PHOTO & (~filters.COMMAND), reject),
                MessageHandler(filters.PHOTO, process)
            ]
        },
        fallbacks = [CommandHandler('cancel', cancel)]
    ))
    
    app.run_polling()